#include "api.h"
#include "deps/sokol_app.h"

static int sys_window_width(lua_State *L) {
    lua_pushnumber(L, sapp_widthf());
    return 1;
}

static int sys_window_height(lua_State *L) {
    lua_pushnumber(L, sapp_heightf());
    return 1;
}

static int sys_quit(lua_State *L) {
    (void)L;
    sapp_request_quit();
    return 0;
}

int sys_lib(lua_State *L) {
    const luaL_Reg libs[] = {
        {"window_width", sys_window_width},
        {"window_height", sys_window_height},
        {"quit", sys_quit},
        {nullptr, nullptr},
    };

    luaL_newlib(L, libs);
    return 1;
}
