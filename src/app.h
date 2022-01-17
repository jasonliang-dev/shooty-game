#pragma once

#include "api.h"
#include "deps/sokol_gfx.h"
#include "deps/sokol_app.h"
#include "renderer.h"

namespace app {

struct State {
    Renderer renderer;
    sg_image white;
    lua_State *lua;
    u64 time_now;
};

extern State *state;

void init();
void event(const sapp_event *e);
void frame();
void cleanup();

} // namespace app
