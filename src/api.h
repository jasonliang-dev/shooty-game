#pragma once

#include "deps/lua/lauxlib.h"
#include "deps/lua/lua.h"
#include "deps/lua/lualib.h"

int gfx_lib(lua_State *L);
int sys_lib(lua_State *L);
int aux_lib(lua_State *L);
int snd_lib(lua_State *L);

static void *luax_field_touserdata(lua_State *L, int arg, const char *key) {
    lua_getfield(L, arg, key);
    void *result = lua_touserdata(L, -1);
    lua_pop(L, 1);
    return result;
}

