#include "api.h"
#include "app.h"

static int snd_play(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);
    ma_engine_play_sound(&app->ma, filename, nullptr);
    return 0;
}

int snd_lib(lua_State *L) {
    const luaL_Reg libs[] = {
        {"play", snd_play},
        {nullptr, nullptr},
    };

    luaL_newlib(L, libs);
    return 1;
}
