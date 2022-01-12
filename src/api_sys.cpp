#include "api.h"
#include "deps/sokol_app.h"

namespace sys {

static int pause(lua_State *L) {
#ifdef _MSC_VER
    __debugbreak();
#else
    #error "pause not supported for platform"
#endif
    return 0;
}

static int window_width(lua_State *L) {
    lua_pushnumber(L, sapp_widthf());
    return 1;
}

static int window_height(lua_State *L) {
    lua_pushnumber(L, sapp_heightf());
    return 1;
}

int lib(lua_State *L) {
    const luaL_Reg libs[] = {
        {"pause", pause},
        {"window_width", window_width},
        {"window_height", window_height},
        {nullptr, nullptr},
    };

    luaL_newlib(L, libs);
    return 1;
}

} // namespace sys
