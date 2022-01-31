#pragma once

#include "deps/sokol_gfx.h"

struct Texture {
    bool try_create(const char *filename);
    void destroy();

    sg_image m_image{};
    int m_width = 0;
    int m_height = 0;
};
