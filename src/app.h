#pragma once

#include "api.h"
#include "deps/miniaudio.h"
#include "deps/sokol_app.h"
#include "renderer.h"
#include "water_renderer.h"

struct AppState {
    Renderer renderer;
    WaterRenderer water;
    sg_image white;
    lua_State *lua;
    ma_engine ma;
    u64 time_now;
};

extern AppState *app;

void app_init();
void app_event(const sapp_event *e);
void app_frame();
void app_cleanup();
