#pragma once

#include "batch_renderer.h"
#include "deps/sokol_gfx.h"
#include "deps/stb_rect_pack.h"
#include "deps/stb_truetype.h"

namespace FontAlign {
enum : unsigned char {
    left = 1 << 0,
    right = 1 << 1,
    center = 1 << 2,
    top = 1 << 3,
    bottom = 1 << 4,
    middle = 1 << 5,
};
}

struct Font {
    Font() = default;
    Font(const char *filename, float size);

    ~Font();

    Font(const Font &) = delete;
    Font &operator=(const Font &) = delete;

    Font(Font &&) noexcept;
    Font &operator=(Font &&) noexcept;

    float width(const char *text) const;
    float height() const;

    struct PrintDesc {
        const char *text;
        float x;
        float y;
        unsigned char color[4];
        unsigned char alignment;
    };

    float print(BatchRenderer &batch, const PrintDesc &desc) const;

  private:
    static constexpr int MAX_CHARS = 256;

    stbtt_packedchar m_ascii[MAX_CHARS]{};
    stbtt_fontinfo m_info{};
    sg_image m_texture{};
    int m_texture_width = 0;
    int m_texture_height = 0;
    float m_size = 0;
};
