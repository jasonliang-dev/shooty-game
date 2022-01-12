#include "api.h"
#include "deps/sokol_gfx.h"
#include "deps/stb_image.h"
#include "font.h"
#include "state.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

namespace gfx {

static int bind_mvp(lua_State *L) {
    Matrix m{};

    lua_pushnil(L);
    for (int i = 0; lua_next(L, 1); i++) {
        m.arr[i] = (float)luaL_checknumber(L, -1);
        lua_pop(L, 1);
    }

    state->batch.mvp(m);
    return 0;
}

static int bind_white_texture(lua_State *L) {
    (void)L;
    state->batch.texture(state->white);
    return 0;
}

static int bind_texture(lua_State *L) {
    unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
    state->batch.texture({id});
    return 0;
}

static int v3_t2(lua_State *L) {
    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    float z = (float)luaL_checknumber(L, 3);

    float u = (float)luaL_checknumber(L, 4);
    float v = (float)luaL_checknumber(L, 5);

    state->batch.push({
        .pos = {x, y, z},
        .uv = {u, v},
        .color = {255, 255, 255, 255},
        .fog = {0, 0, 0, 0},
    });

    return 0;
}

static int make_texture(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);

    int width;
    int height;
    int channels;
    void *data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data) {
        return 0;
    }
    assert(channels == 4);

    sg_image_desc desc{};
    desc.width = width;
    desc.height = height;
    desc.pixel_format = SG_PIXELFORMAT_RGBA8;
    desc.data.subimage[0][0].ptr = data;
    desc.data.subimage[0][0].size = width * height * channels;

    sg_image image = sg_make_image(desc);
    stbi_image_free(data);

    lua_createtable(L, 0, 4);

    lua_pushinteger(L, image.id);
    lua_setfield(L, -2, "id");

    lua_pushinteger(L, width);
    lua_setfield(L, -2, "width");

    lua_pushinteger(L, height);
    lua_setfield(L, -2, "height");

    lua_createtable(L, 0, 1);
    lua_pushcfunction(L, [](lua_State *L) -> int {
        unsigned int id = (unsigned int)luax::field_checkinteger(L, 1, "id");
        sg_destroy_image(sg_image{id});
        return 0;
    });
    lua_setfield(L, -2, "__gc");
    lua_setmetatable(L, -2);

    return 1;
}

static int make_font(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);
    const float size = (float)luaL_checknumber(L, 2);

    lua_createtable(L, 0, 3);

    Font *font = new Font(filename, size);
    // printf("%p\n", (void *)font);
    lua_pushlightuserdata(L, font);
    lua_setfield(L, -2, "udata");

    lua_pushcfunction(L, [](lua_State *L) -> int {
        Font *font = (Font *)luax::field_touserdata(L, 1, "udata");
        // printf("%p\n", (void *)font);

        const char *text = luaL_checkstring(L, 2);
        float x = (float)luaL_checknumber(L, 3);
        float y = (float)luaL_checknumber(L, 4);

        // can't brace initialize?
        Font::PrintDesc desc;
        desc.text = text;
        desc.x = x;
        desc.y = y;
        memset(desc.color, 255, sizeof(unsigned char) * 4);
        desc.alignment = FontAlign::left | FontAlign::bottom;

        float n = font->print(state->batch, desc);

        lua_pushnumber(L, n);
        return 1;
    });
    lua_setfield(L, -2, "print");

    lua_createtable(L, 0, 1);
    lua_pushcfunction(L, [](lua_State *L) -> int {
        Font *font = (Font *)luax::field_touserdata(L, 1, "udata");
        delete font;
        return 0;
    });
    lua_setfield(L, -2, "__gc");
    lua_setmetatable(L, -2);

    return 1;
}

int lib(lua_State *L) {
    const luaL_Reg libs[] = {
        {"bind_mvp", bind_mvp},
        {"bind_white_texture", bind_white_texture},
        {"bind_texture", bind_texture},
        {"v3_t2", v3_t2},
        {"make_texture", make_texture},
        {"make_font", make_font},
        {nullptr, nullptr},
    };

    luaL_newlib(L, libs);
    return 1;
}

} // namespace gfx
