#pragma once

#include "deps/sokol_gfx.h"
#include "deps/stb_rect_pack.h"
#include "deps/stb_truetype.h"
#include "renderer.h"
#include "texture.h"

struct FontPrintDesc {
    const char *text;
    float x;
    float y;
    u8 color[4];
};

struct Font {
    const char *try_create(const char *filename, float size);
    void destroy();

    float width(const char *text) const;
    float print(Renderer &renderer, const FontPrintDesc &desc) const;

    float height() const;
    float size() const;
    float baseline() const;

  private:
    static constexpr int MAX_CHARS = 256;

    stbtt_packedchar m_ascii[MAX_CHARS]{};
    stbtt_fontinfo m_info{};
    Texture m_texture{};
    float m_size = 0;
    float m_scale = 0;
    int m_ascent = 0;
    int m_descent = 0;
    int m_linegap = 0;
};
