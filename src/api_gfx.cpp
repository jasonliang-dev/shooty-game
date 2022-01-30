#include "api.h"
#include "app.h"
#include "deps/sokol_app.h"
#include "deps/sokol_gfx.h"
#include "deps/stb_image.h"
#include "font.h"
#include "tilemap.h"
#include <stdio.h>
#include <string.h>

static int gfx_begin_draw(lua_State *L) {
    sg_begin_default_pass(sg_pass_action{}, sapp_width(), sapp_height());
    app->renderer.begin();
    return 0;
}

static int gfx_end_draw(lua_State *L) {
    app->renderer.flush();
    sg_end_pass();
    sg_commit();
    return 0;
}

static int gfx_draw_count(lua_State *L) {
    lua_pushnumber(L, app->renderer.draw_count());
    return 1;
}

static int gfx_bind_mvp(lua_State *L) {
    RenMatrix m{};

    lua_pushnil(L);
    for (int i = 0; lua_next(L, 1); i++) {
        m.arr[i] = (float)luaL_checknumber(L, -1);
        lua_pop(L, 1);
    }

    app->renderer.mvp(m);
    return 0;
}

static int gfx_bind_white_texture(lua_State *L) {
    (void)L;
    app->renderer.texture(app->white);
    return 0;
}

static int gfx_bind_texture(lua_State *L) {
    u32 id = (u32)luaL_checkinteger(L, 1);
    app->renderer.texture({id});
    return 0;
}

static int gfx_v3_t2(lua_State *L) {
    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    float z = (float)luaL_checknumber(L, 3);
    float u = (float)luaL_checknumber(L, 4);
    float v = (float)luaL_checknumber(L, 5);

    app->renderer.push({
        .pos = {x, y, z},
        .uv = {u, v},
        .color = {255, 255, 255, 255},
        .fog = {0, 0, 0, 0},
    });

    return 0;
}

static int gfx_v3_t2_c4(lua_State *L) {
    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    float z = (float)luaL_checknumber(L, 3);
    float u = (float)luaL_optnumber(L, 4, 0);
    float v = (float)luaL_optnumber(L, 5, 0);
    u8 r = (u8)luaL_optinteger(L, 6, 255);
    u8 g = (u8)luaL_optinteger(L, 7, 255);
    u8 b = (u8)luaL_optinteger(L, 8, 255);
    u8 a = (u8)luaL_optinteger(L, 9, 255);

    app->renderer.push({
        .pos = {x, y, z},
        .uv = {u, v},
        .color = {r, g, b, a},
        .fog = {0, 0, 0, 0},
    });

    return 0;
}

static int gfx_v3_t2_c4_f4(lua_State *L) {
    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    float z = (float)luaL_checknumber(L, 3);
    float u = (float)luaL_optnumber(L, 4, 0);
    float v = (float)luaL_optnumber(L, 5, 0);
    u8 r = (u8)luaL_optinteger(L, 6, 255);
    u8 g = (u8)luaL_optinteger(L, 7, 255);
    u8 b = (u8)luaL_optinteger(L, 8, 255);
    u8 a = (u8)luaL_optinteger(L, 9, 255);
    u8 fr = (u8)luaL_optinteger(L, 10, 0);
    u8 fg = (u8)luaL_optinteger(L, 11, 0);
    u8 fb = (u8)luaL_optinteger(L, 12, 0);
    u8 fa = (u8)luaL_optinteger(L, 13, 0);

    app->renderer.push({
        .pos = {x, y, z},
        .uv = {u, v},
        .color = {r, g, b, a},
        .fog = {fr, fg, fb, fa},
    });

    return 0;
}

static int gfx_make_texture(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);

    int width;
    int height;
    int channels;
    void *data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data || channels != 4) {
        return 0;
    }

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
        lua_getfield(L, 1, "id");
        u32 id = (u32)luaL_checkinteger(L, -1);
        lua_pop(L, 1);

        sg_destroy_image(sg_image{id});
        return 0;
    });
    lua_setfield(L, -2, "__gc");
    lua_setmetatable(L, -2);

    return 1;
}

static int gfx_make_font(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);
    const float size = (float)luaL_checknumber(L, 2);

    lua_createtable(L, 0, 3);

    Font *font = new Font;
    bool ok = font->try_create(filename, size);
    if (!ok) {
        delete font;
        return 0;
    }

    lua_pushlightuserdata(L, font);
    lua_setfield(L, -2, "udata");

    lua_pushcfunction(L, [](lua_State *L) -> int {
        Font *font = (Font *)luax_field_touserdata(L, 1, "udata");

        const char *text = luaL_checkstring(L, 2);
        float x = (float)luaL_checknumber(L, 3);
        float y = (float)luaL_checknumber(L, 4);

        // can't brace initialize?
        Font::PrintDesc desc{};
        desc.text = text;
        desc.x = x;
        desc.y = y;
        memset(desc.color, 255, sizeof(u8) * 4);
        desc.alignment = FONT_ALIGN_LEFT | FONT_ALIGN_BOTTOM;

        float n = font->print(app->renderer, desc);

        lua_pushnumber(L, n);
        return 1;
    });
    lua_setfield(L, -2, "print");

    lua_createtable(L, 0, 1);
    lua_pushcfunction(L, [](lua_State *L) -> int {
        Font *font = (Font *)luax_field_touserdata(L, 1, "udata");
        font->destroy();
        delete font;
        return 0;
    });
    lua_setfield(L, -2, "__gc");
    lua_setmetatable(L, -2);

    return 1;
}

static int gfx_make_tilemap(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);

    lua_createtable(L, 0, 3);

    Tilemap *map = new Tilemap;
    bool ok = map->try_create(filename);
    if (!ok) {
        delete map;
        return 0;
    }

    lua_pushlightuserdata(L, map);
    lua_setfield(L, -2, "udata");

    lua_pushcfunction(L, [](lua_State *L) -> int {
        Tilemap *map = (Tilemap *)luax_field_touserdata(L, 1, "udata");

        RenMatrix mat{};
        for (int i = 0; i < 16; i++) {
            lua_rawgeti(L, 2, i + 1);
            mat.arr[i] = (float)luaL_checknumber(L, -1);
            lua_pop(L, 1);
        }

        u32 id = (u32)luaL_checkinteger(L, 3);

        map->draw(app->renderer, mat, sg_image{id});
        return 0;
    });
    lua_setfield(L, -2, "draw");

    lua_createtable(L, 0, 1);
    lua_pushcfunction(L, [](lua_State *L) -> int {
        Tilemap *map = (Tilemap *)luax_field_touserdata(L, 1, "udata");
        map->destroy();
        delete map;
        return 0;
    });
    lua_setfield(L, -2, "__gc");
    lua_setmetatable(L, -2);

    return 1;
}

int gfx_lib(lua_State *L) {
    const luaL_Reg libs[] = {
        {"begin_draw", gfx_begin_draw},
        {"end_draw", gfx_end_draw},
        {"draw_count", gfx_draw_count},
        {"bind_mvp", gfx_bind_mvp},
        {"bind_white_texture", gfx_bind_white_texture},
        {"bind_texture", gfx_bind_texture},
        {"v3_t2", gfx_v3_t2},
        {"v3_t2_c4", gfx_v3_t2_c4},
        {"v3_t2_c4_f4", gfx_v3_t2_c4_f4},
        {"make_texture", gfx_make_texture},
        {"make_font", gfx_make_font},
        {"make_tilemap", gfx_make_tilemap},
        {nullptr, nullptr},
    };

    luaL_newlib(L, libs);
    return 1;
}
