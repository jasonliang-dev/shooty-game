#pragma once

#include "deps/sokol_gfx.h"
#include "types.h"

struct WaterRenderer {
    void create();
    void destroy();

    void draw(Matrix mvp, float scale, float time);

  private:
    sg_pipeline m_pip{};
    sg_buffer m_vbo{};
};
