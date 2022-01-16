#pragma once

#include "api.h"
#include "renderer.h"
#include "deps/sokol_gfx.h"

struct State {
    Renderer renderer;
    sg_image white;
    lua_State *lua;
    u64 time_now;
};

extern State *state;
