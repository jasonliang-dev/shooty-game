#define _CRT_SECURE_NO_WARNINGS

#include "font.h"
#include "deps/sokol_gl.h"
#include "io.h"
#include <stdio.h>
#include <string.h>

Font::Font(const char *filename, float size) {
    const auto read_file = [](const char *filename) -> unsigned char * {
        FILE *handle = fopen(filename, "rb");
        if (!handle) {
            return nullptr;
        }

        fseek(handle, 0, SEEK_END);
        int size = ftell(handle);
        fseek(handle, 0, SEEK_SET);

        unsigned char *data = new unsigned char[size];
        fread(data, 1, size, handle);
        fclose(handle);
        return data;
    };

    unsigned char *ttf = read_file(filename);
    int ok = stbtt_InitFont(&m_info, ttf, 0);
    if (!ok) {
        return;
    }

    const int width = 1024;
    const int height = 1024;
    unsigned char *bitmap = new unsigned char[width * height];

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

    unsigned char *rgba = new unsigned char[width * height * 4];
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

    m_texture = sg_make_image(desc);
    m_texture_width = width;
    m_texture_height = height;
    m_size = size;

    delete[] ttf;
    delete[] bitmap;
    delete[] rgba;
}

Font::~Font() { sg_destroy_image(m_texture); }

Font::Font(Font &&other) noexcept {
    memcpy(m_ascii, other.m_ascii, sizeof(m_ascii));
    m_info = other.m_info;
    m_texture = other.m_texture;
    m_texture_width = other.m_texture_width;
    m_texture_height = other.m_texture_height;
    m_size = other.m_size;

    other.m_texture.id = 0;
}

Font &Font::operator=(Font &&other) noexcept {
    if (this != &other) {
        sg_destroy_image(m_texture);

        memcpy(m_ascii, other.m_ascii, sizeof(m_ascii));
        m_info = other.m_info;
        m_texture = other.m_texture;
        m_texture_width = other.m_texture_width;
        m_texture_height = other.m_texture_height;
        m_size = other.m_size;

        other.m_texture.id = 0;
    }

    return *this;
}

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

float Font::print(BatchRenderer &batch, const PrintDesc &desc) const {
    const char *text = desc.text;

    float x = desc.x;
    if (desc.alignment & FontAlign::right) {
        x -= width(text);
    } else if (desc.alignment & FontAlign::center) {
        x -= width(text) / 2;
    }

    float y = desc.y;
    if (desc.alignment & FontAlign::top) {
        y += m_size;
    } else if (desc.alignment & FontAlign::middle) {
        y += m_size / 2;
    }

    unsigned char r = desc.color[0];
    unsigned char g = desc.color[1];
    unsigned char b = desc.color[2];
    unsigned char a = desc.color[3];
    batch.texture(m_texture);
    while (*text) {
        stbtt_aligned_quad q{};
        stbtt_GetPackedQuad(m_ascii, m_texture_width, m_texture_height,
                            *text, &x, &y, &q, 0);
        batch.push({{q.x0, q.y0, 0}, {q.s0, q.t0}, {r, g, b, a}, {}});
        batch.push({{q.x0, q.y1, 0}, {q.s0, q.t1}, {r, g, b, a}, {}});
        batch.push({{q.x1, q.y1, 0}, {q.s1, q.t1}, {r, g, b, a}, {}});
        batch.push({{q.x1, q.y0, 0}, {q.s1, q.t0}, {r, g, b, a}, {}});
        text++;
    }

    return x;
}
