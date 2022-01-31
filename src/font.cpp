#define _CRT_SECURE_NO_WARNINGS

#include "font.h"
#include "deps/sokol_gl.h"
#include "types.h"
#include <stdio.h>
#include <string.h>

bool Font::try_create(const char *filename, float size) {
    const auto read_file = [](const char *filename) -> u8 * {
        FILE *handle = fopen(filename, "rb");
        if (!handle) {
            return nullptr;
        }

        fseek(handle, 0, SEEK_END);
        int size = ftell(handle);
        fseek(handle, 0, SEEK_SET);

        u8 *data = new u8[size];
        fread(data, 1, size, handle);
        fclose(handle);
        return data;
    };

    u8 *ttf = read_file(filename);
    int ok = stbtt_InitFont(&m_info, ttf, 0);
    if (!ok) {
        return false;
    }

    const int width = 1024;
    const int height = 1024;
    u8 *bitmap = new u8[width * height];

    stbtt_pack_context spc{};
    stbtt_PackBegin(&spc, bitmap, width, height, 0, 2, nullptr);
    stbtt_PackSetOversampling(&spc, 2, 2);

    stbtt_pack_range range = {
        .font_size = size,
        .first_unicode_codepoint_in_range = 0,
        .array_of_unicode_codepoints = nullptr,
        .num_chars = MAX_CHARS,
        .chardata_for_range = m_ascii,
    };

    stbtt_PackFontRanges(&spc, ttf, 0, &range, 1);
    stbtt_PackEnd(&spc);

    m_info.data = nullptr;

    u8 *rgba = new u8[width * height * 4];
    for (int i = 0; i < width * height; i++) {
        rgba[i * 4 + 0] = 255;
        rgba[i * 4 + 1] = 255;
        rgba[i * 4 + 2] = 255;
        rgba[i * 4 + 3] = bitmap[i];
    }

    sg_image_desc desc{};
    desc.width = width;
    desc.height = height;
    desc.pixel_format = SG_PIXELFORMAT_RGBA8;
    desc.min_filter = SG_FILTER_NEAREST;
    desc.mag_filter = SG_FILTER_NEAREST;
    desc.data.subimage[0][0].ptr = rgba;
    desc.data.subimage[0][0].size = width * height * 4;

    m_texture.m_image = sg_make_image(desc);
    m_texture.m_width = width;
    m_texture.m_height = height;
    m_size = size;

    delete[] ttf;
    delete[] bitmap;
    delete[] rgba;

    return true;
}

void Font::destroy() { m_texture.destroy(); }

float Font::width(const char *text) const {
    float width = 0;

    while (*text) {
        const stbtt_packedchar *packed = m_ascii + *text;
        width += packed->xadvance;
        text++;
    }

    return width;
}

float Font::height() const { return m_size; }

float Font::print(Renderer &renderer, const PrintDesc &desc) const {
    const char *text = desc.text;

    float x = desc.x;
    if (desc.alignment & FONT_ALIGN_RIGHT) {
        x -= width(text);
    } else if (desc.alignment & FONT_ALIGN_CENTER) {
        x -= width(text) / 2;
    }

    float y = desc.y;
    if (desc.alignment & FONT_ALIGN_TOP) {
        y += m_size;
    } else if (desc.alignment & FONT_ALIGN_MIDDLE) {
        y += m_size / 2;
    }

    u8 r = desc.color[0];
    u8 g = desc.color[1];
    u8 b = desc.color[2];
    u8 a = desc.color[3];
    renderer.texture(m_texture.m_image);
    while (*text) {
        stbtt_aligned_quad q{};
        stbtt_GetPackedQuad(m_ascii, m_texture.m_width, m_texture.m_height,
                            *text, &x, &y, &q, 0);
        renderer.push({{q.x0, q.y0, 0}, {q.s0, q.t0}, {r, g, b, a}, {}});
        renderer.push({{q.x0, q.y1, 0}, {q.s0, q.t1}, {r, g, b, a}, {}});
        renderer.push({{q.x1, q.y1, 0}, {q.s1, q.t1}, {r, g, b, a}, {}});
        renderer.push({{q.x1, q.y0, 0}, {q.s1, q.t0}, {r, g, b, a}, {}});
        text++;
    }

    return x;
}
