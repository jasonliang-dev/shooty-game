#pragma once

#include "api.h"
#include "batch_renderer.h"
#include "deps/sokol_gfx.h"

struct State {
    BatchRenderer batch;
    sg_image white;
    lua_State *lua;
    u64 time_now;
};

extern State *state;
