#include "app.h"
#include "api.h"
#include "deps/sokol_glue.h"
#include "deps/sokol_time.h"
#include <stdlib.h>

AppState *app;

void app_init(void) {
    sg_setup({.context = sapp_sgcontext()});
    stm_setup();
    app = new AppState{};

    lua_State *L = app->lua = luaL_newstate();
    luaL_openlibs(L);
    luaL_requiref(L, "sys", sys_lib, true);
    luaL_requiref(L, "gfx", gfx_lib, true);
    luaL_requiref(L, "aux", aux_lib, true);
    lua_pop(L, 3);

    lua_newtable(L);
    lua_setglobal(L, "core");

    const char *bootstrap = "xpcall(function()\n"
                            "    package.path = 'data/?.lua'\n"
                            "    require 'main'\n"
                            "    core.init()\n"
                            "end, function(err)\n"
                            "    print(tostring(err))\n"
                            "    print(debug.traceback(nil, 2))\n"
                            "    os.exit(-1)\n"
                            "end)\n";
    if (luaL_dostring(L, bootstrap) != LUA_OK) {
        fprintf(stderr, "lua bootstrap failed\n");
        abort();
    }

    // message handler when using lua_pcall
    // always at bottom of lua stack
    lua_pushcfunction(L, [](lua_State *L) -> int {
        // print(tostring(err))
        lua_getglobal(L, "print");
        lua_getglobal(L, "tostring");
        lua_pushvalue(L, 1);
        lua_call(L, 1, 1);
        lua_call(L, 1, 0);

        // print(debug.traceback(nil, 2))
        lua_getglobal(L, "print");
        lua_getglobal(L, "debug");
        lua_getfield(L, -1, "traceback");
        lua_remove(L, -2);
        lua_pushnil(L);
        lua_pushinteger(L, 2);
        lua_call(L, 2, 1);
        lua_call(L, 1, 0);

        exit(-1);
    });

    app->renderer.create(9000);

    u8 rgba[4] = {255, 255, 255, 255};
    sg_image_data image_data{};
    image_data.subimage[0][0] = SG_RANGE(rgba);
    app->white = sg_make_image({
        .width = 1,
        .height = 1,
        .pixel_format = SG_PIXELFORMAT_RGBA8,
        .min_filter = SG_FILTER_NEAREST,
        .mag_filter = SG_FILTER_NEAREST,
        .data = image_data,
    });

    app->time_now = stm_now();
}

void app_event(const sapp_event *e) {
    lua_State *L = app->lua;

    // xpcall(core.event, msgh, ...)
    lua_getglobal(L, "core");
    lua_getfield(L, -1, "event");
    lua_remove(L, -2);

    switch (e->type) {
    case SAPP_EVENTTYPE_KEY_DOWN: {
        lua_pushstring(L, "keydown");
        lua_pushinteger(L, e->key_code);
        lua_pcall(L, 2, 0, 1);
        if (e->key_code == SAPP_KEYCODE_ESCAPE) {
            sapp_request_quit();
        }
        break;
    }
    case SAPP_EVENTTYPE_KEY_UP: {
        lua_pushstring(L, "keyup");
        lua_pushinteger(L, e->key_code);
        lua_pcall(L, 2, 0, 1);
        break;
    }
    case SAPP_EVENTTYPE_MOUSE_DOWN: {
        lua_pushstring(L, "mousedown");
        lua_pushinteger(L, e->mouse_button);
        lua_pcall(L, 2, 0, 1);
        break;
    }
    case SAPP_EVENTTYPE_MOUSE_UP: {
        lua_pushstring(L, "mouseup");
        lua_pushinteger(L, e->mouse_button);
        lua_pcall(L, 2, 0, 1);
        break;
    }
    case SAPP_EVENTTYPE_MOUSE_MOVE: {
        lua_pushstring(L, "mousemove");
        lua_pushnumber(L, e->mouse_x);
        lua_pushnumber(L, e->mouse_y);
        lua_pushnumber(L, e->mouse_dx);
        lua_pushnumber(L, e->mouse_dy);
        lua_pcall(L, 5, 0, 1);
        break;
    }
    case SAPP_EVENTTYPE_MOUSE_SCROLL: {
        lua_pushstring(L, "mousescroll");
        lua_pushnumber(L, e->scroll_x);
        lua_pushnumber(L, e->scroll_y);
        lua_pcall(L, 3, 0, 1);
        break;
    }
    default:
        lua_pop(L, 1);
        break;
    }
}

void app_frame(void) {
    u64 now = stm_now();
    float dt = stm_diff(now, app->time_now) / 1000000000.0f;
    app->time_now = now;

    lua_State *L = app->lua;

    // xpcall(core.frame, msgh)
    lua_getglobal(L, "core");
    lua_getfield(L, -1, "frame");
    lua_remove(L, -2);
    lua_pushnumber(L, dt);

    if (lua_pcall(L, 1, 0, 1) != LUA_OK) [[unlikely]] {
        fprintf(stderr, "core.frame failed\n");
        abort();
    }
}

void app_cleanup(void) {
    lua_close(app->lua);
    app->renderer.destroy();
    delete app;
    sg_shutdown();
}
