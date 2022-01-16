#define LUA_LIB

#include "api.h"
#include "state.h"

#define SOKOL_IMPL
#define SOKOL_GLCORE33
#define SOKOL_WIN32_FORCE_MAIN
#include "deps/sokol_app.h"
#include "deps/sokol_gfx.h"
#include "deps/sokol_glue.h"
#include "deps/sokol_time.h"

#define MAKE_LIB
#include "deps/lua/onelua.c"

#define STB_IMAGE_IMPLEMENTATION
#include "deps/stb_image.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "deps/stb_rect_pack.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "deps/stb_truetype.h"

State *state; // extern state.h

void init(void) {
    sg_setup({.context = sapp_sgcontext()});
    stm_setup();
    state = new State{};

    lua_State *L = state->lua = luaL_newstate();
    luaL_openlibs(L);
    luaL_requiref(L, "sys", sys::lib, true);
    luaL_requiref(L, "gfx", gfx::lib, true);
    lua_pop(L, 2);

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

    state->renderer = Renderer(6000);

    u8 rgba[4] = {255, 255, 255, 255};
    sg_image_data image_data{};
    image_data.subimage[0][0] = SG_RANGE(rgba);
    state->white = sg_make_image({
        .width = 1,
        .height = 1,
        .pixel_format = SG_PIXELFORMAT_RGBA8,
        .min_filter = SG_FILTER_NEAREST,
        .mag_filter = SG_FILTER_NEAREST,
        .data = image_data,
    });

    state->time_now = stm_now();
}

void event(const sapp_event *e) {
    lua_State *L = state->lua;

    // xpcall(core.event, l_msgh, ...)
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
        lua_pushinteger(L, e->mouse_dx);
        lua_pushinteger(L, e->mouse_dy);
        lua_pcall(L, 3, 0, 1);
        break;
    }
    default:
        lua_pop(L, 1);
        break;
    }
}

void frame(void) {
    u64 now = stm_now();
    float dt = stm_diff(now, state->time_now) / 1000000000.0f;
    state->time_now = now;

    lua_State *L = state->lua;

    // xpcall(core.frame, l_msgh)
    lua_getglobal(L, "core");
    lua_getfield(L, -1, "frame");
    lua_remove(L, -2);
    lua_pushnumber(L, dt);

    if (lua_pcall(L, 1, 0, 1) != LUA_OK) [[unlikely]] {
        fprintf(stderr, "core.frame failed\n");
        abort();
    }
}

void cleanup(void) {
    lua_close(state->lua);
    delete state;
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    return {
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .width = 1366,
        .height = 768,
        .sample_count = 4,
        .window_title = "This is a title",
        .icon = {.sokol_default = true},
        .win32_console_attach = true,
    };
}
