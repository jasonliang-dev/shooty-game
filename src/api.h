#pragma once

#include "deps/lua/lauxlib.h"
#include "deps/lua/lua.h"
#include "deps/lua/lualib.h"

namespace gfx {
int lib(lua_State *L);
}

namespace sys {
int lib(lua_State *L);
}

namespace luax {

static lua_Integer field_checkinteger(lua_State *L, int arg, const char *key) {
    lua_getfield(L, arg, key);
    lua_Integer result = luaL_checkinteger(L, -1);
    lua_pop(L, 1);
    return result;
}

static void *field_touserdata(lua_State *L, int arg, const char *key) {
    lua_getfield(L, arg, key);
    void *result = lua_touserdata(L, -1);
    lua_pop(L, 1);
    return result;
}

} // namespace luax
