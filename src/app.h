#pragma once

#include "api.h"
#include "deps/sokol_app.h"
#include "renderer.h"

struct AppState {
    Renderer renderer;
    sg_image white;
    lua_State *lua;
    u64 time_now;
};

extern AppState *app;

void app_init();
void app_event(const sapp_event *e);
void app_frame();
void app_cleanup();

