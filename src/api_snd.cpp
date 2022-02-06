#include "api.h"
#include "app.h"

static int snd_play(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);
    ma_engine_play_sound(&app->ma, filename, nullptr);
    return 0;
}

static int snd_master_volume(lua_State *L) {
    float vol = (float)luaL_checknumber(L, 1);
    ma_engine_set_volume(&app->ma, vol);
    return 0;
}

int snd_lib(lua_State *L) {
    const luaL_Reg libs[] = {
        {"play", snd_play},
        {"master_volume", snd_master_volume},
        {nullptr, nullptr},
    };

    luaL_newlib(L, libs);
    return 1;
}
