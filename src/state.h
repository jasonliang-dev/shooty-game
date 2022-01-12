#pragma once

#include "api.h"
#include "batch_renderer.h"
#include "deps/sokol_gfx.h"

struct State {
    BatchRenderer batch;
    sg_image white;
    lua_State *lua;
    unsigned long long time_now;
};

extern State *state;
