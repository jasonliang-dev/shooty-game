#pragma once

#include "deps/lua/lauxlib.h"
#include "deps/lua/lua.h"
#include "deps/lua/lualib.h"

int gfx_lib(lua_State *L);
int sys_lib(lua_State *L);
int aux_lib(lua_State *L);
int snd_lib(lua_State *L);
