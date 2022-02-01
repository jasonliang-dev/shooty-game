#pragma once

#include "deps/sokol_gfx.h"
#include "deps/stb_rect_pack.h"
#include "deps/stb_truetype.h"
#include "renderer.h"
#include "texture.h"

enum FontAlign : u8 {
    FONT_ALIGN_LEFT = 1 << 0,
    FONT_ALIGN_RIGHT = 1 << 1,
    FONT_ALIGN_CENTER = 1 << 2,
    FONT_ALIGN_TOP = 1 << 3,
    FONT_ALIGN_BOTTOM = 1 << 4,
    FONT_ALIGN_MIDDLE = 1 << 5,
};

struct Font {
    const char *try_create(const char *filename, float size);
    void destroy();

    float width(const char *text) const;
    float height() const;

    struct PrintDesc {
        const char *text;
        float x;
        float y;
        u8 color[4];
        u8 alignment;
    };
    float print(Renderer &renderer, const PrintDesc &desc) const;

  private:
    static constexpr int MAX_CHARS = 256;

    stbtt_packedchar m_ascii[MAX_CHARS]{};
    stbtt_fontinfo m_info{};
    Texture m_texture{};
    float m_size = 0;
};
