#include "api.h"
#include "app.h"
#include "font.h"
#include "texture.h"
#include "tilemap.h"
#include "tileset.h"
#include <string.h>

static void *field_touserdata(lua_State *L, int arg, const char *key) {
    lua_getfield(L, arg, key);
    void *result = lua_touserdata(L, -1);
    lua_pop(L, 1);
    return result;
}

static int aux_make_texture(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);

    Texture tex;
    const char *err = tex.try_create(filename);
    if (err) {
        return luaL_error(L, "%s", err);
    }

    lua_createtable(L, 0, 4);

    lua_pushinteger(L, tex.m_image.id);
    lua_setfield(L, -2, "id");

    lua_pushinteger(L, tex.m_width);
    lua_setfield(L, -2, "width");

    lua_pushinteger(L, tex.m_height);
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

static int aux_make_font(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);
    const float size = (float)luaL_checknumber(L, 2);

    Font *font = new Font;
    const char *err = font->try_create(filename, size);
    if (err) {
        delete font;
        return luaL_error(L, "%s", err);
    }

    lua_createtable(L, 0, 3);

    lua_pushlightuserdata(L, font);
    lua_setfield(L, -2, "udata");

    lua_pushnumber(L, font->size());
    lua_setfield(L, -2, "size");

    lua_pushnumber(L, font->height());
    lua_setfield(L, -2, "height");

    lua_pushnumber(L, font->baseline());
    lua_setfield(L, -2, "baseline");

    lua_pushcfunction(L, [](lua_State *L) -> int {
        Font *font = (Font *)field_touserdata(L, 1, "udata");
        const char *text = luaL_checkstring(L, 2);
        lua_pushnumber(L, font->width(text));
        return 1;
    });
    lua_setfield(L, -2, "width");

    lua_pushcfunction(L, [](lua_State *L) -> int {
        Font *font = (Font *)field_touserdata(L, 1, "udata");

        const char *text = luaL_checkstring(L, 2);
        float x = (float)luaL_checknumber(L, 3);
        float y = (float)luaL_checknumber(L, 4);
        u8 r = (u8)luaL_optinteger(L, 5, 255);
        u8 g = (u8)luaL_optinteger(L, 6, 255);
        u8 b = (u8)luaL_optinteger(L, 7, 255);
        u8 a = (u8)luaL_optinteger(L, 8, 255);

        FontPrintDesc desc{};
        desc.text = text;
        desc.x = x;
        desc.y = y;
        desc.color[0] = r;
        desc.color[1] = g;
        desc.color[2] = b;
        desc.color[3] = a;

        float n = font->print(app->renderer, desc);

        lua_pushnumber(L, n);
        return 1;
    });
    lua_setfield(L, -2, "print");

    lua_createtable(L, 0, 1);
    lua_pushcfunction(L, [](lua_State *L) -> int {
        Font *font = (Font *)field_touserdata(L, 1, "udata");
        font->destroy();
        delete font;
        return 0;
    });
    lua_setfield(L, -2, "__gc");
    lua_setmetatable(L, -2);

    return 1;
}

static int aux_make_tileset(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);

    Tileset *tileset = new Tileset;
    const char *err = tileset->try_create(filename);
    if (err) {
        delete tileset;
        return luaL_error(L, "%s", err);
    }

    lua_createtable(L, 0, 3);

    lua_pushlightuserdata(L, tileset);
    lua_setfield(L, -2, "udata");

    lua_pushinteger(L, tileset->image().id);
    lua_setfield(L, -2, "texture_id");

    lua_createtable(L, 0, 1);
    lua_pushcfunction(L, [](lua_State *L) -> int {
        Tileset *tileset = (Tileset *)field_touserdata(L, 1, "udata");
        tileset->destroy();
        delete tileset;
        return 0;
    });
    lua_setfield(L, -2, "__gc");
    lua_setmetatable(L, -2);

    return 1;
}

static int aux_make_tilemap(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);
    Tileset *tileset = (Tileset *)field_touserdata(L, 2, "udata");

    Tilemap *map = new Tilemap;
    const char *err = map->try_create(filename, *tileset);
    if (err) {
        delete map;
        return luaL_error(L, "%s", err);
    }

    lua_createtable(L, 0, 9);

    lua_pushlightuserdata(L, map);
    lua_setfield(L, -2, "udata");

    lua_pushcfunction(L, [](lua_State *L) -> int {
        Tilemap *map = (Tilemap *)field_touserdata(L, 1, "udata");

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

    lua_pushcfunction(L, [](lua_State *L) -> int {
        Tilemap *map = (Tilemap *)field_touserdata(L, 1, "udata");

        float x = (float)luaL_checknumber(L, 2);
        float y = (float)luaL_checknumber(L, 3);
        lua_pushboolean(L, map->point_collision(x, y));
        return 1;
    });
    lua_setfield(L, -2, "point_collision");

    lua_pushcfunction(L, [](lua_State *L) -> int {
        Tilemap *map = (Tilemap *)field_touserdata(L, 1, "udata");

        float x = (float)luaL_checknumber(L, 2);
        float y = (float)luaL_checknumber(L, 3);
        float dx = (float)luaL_checknumber(L, 4);
        float dy = (float)luaL_checknumber(L, 5);
        int sub_steps = (int)luaL_checkinteger(L, 6);

        TilemapPointMoveResult res =
            map->point_move(vec2(x, y), vec2(dx, dy), sub_steps);
        lua_pushnumber(L, res.x);
        lua_pushnumber(L, res.y);
        lua_pushboolean(L, res.collided);
        return 3;
    });
    lua_setfield(L, -2, "point_move");

    lua_pushcfunction(L, [](lua_State *L) -> int {
        Tilemap *map = (Tilemap *)field_touserdata(L, 1, "udata");
        const char *type = luaL_checkstring(L, 2);
        TilemapObject obj = map->object_by_type(type);

        if (!obj.type[0]) {
            return 0;
        }

        lua_createtable(L, 0, 3);

        lua_pushnumber(L, obj.x);
        lua_setfield(L, -2, "x");

        lua_pushnumber(L, obj.y);
        lua_setfield(L, -2, "y");

        lua_pushstring(L, obj.type);
        lua_setfield(L, -2, "type");

        return 1;
    });
    lua_setfield(L, -2, "object_by_type");

    lua_pushcfunction(L, [](lua_State *L) -> int {
        Tilemap *map = (Tilemap *)field_touserdata(L, 1, "udata");
        const char *type = luaL_checkstring(L, 2);

        TilemapObject *objs = nullptr;
        int object_count = map->objects(objs);

        lua_newtable(L);

        int len = 0;
        for (int i = 0; i < object_count; i++) {
            if (strcmp(objs[i].type, type) == 0) {
                lua_createtable(L, 0, 3);

                lua_pushnumber(L, objs[i].x);
                lua_setfield(L, -2, "x");

                lua_pushnumber(L, objs[i].y);
                lua_setfield(L, -2, "y");

                lua_pushstring(L, objs[i].type);
                lua_setfield(L, -2, "type");

                lua_rawseti(L, -2, len + 1);
                len++;
            }
        }

        return 1;
    });
    lua_setfield(L, -2, "objects_by_type");

    lua_pushcfunction(L, [](lua_State *L) -> int {
        Tilemap *map = (Tilemap *)field_touserdata(L, 1, "udata");
        TilemapObject *objs = nullptr;
        int object_count = map->objects(objs);

        lua_createtable(L, object_count, 0);
        for (int i = 0; i < object_count; i++) {
            lua_createtable(L, 0, 3);

            lua_pushnumber(L, objs[i].x);
            lua_setfield(L, -2, "x");

            lua_pushnumber(L, objs[i].y);
            lua_setfield(L, -2, "y");

            lua_pushstring(L, objs[i].type);
            lua_setfield(L, -2, "type");

            lua_rawseti(L, -2, i + 1);
        }

        return 1;
    });
    lua_setfield(L, -2, "objects");

    lua_pushcfunction(L, [](lua_State *L) -> int {
        Tilemap *map = (Tilemap *)field_touserdata(L, 1, "udata");

        int start_x = (int)luaL_checknumber(L, 2);
        int start_y = (int)luaL_checknumber(L, 3);
        int end_x = (int)luaL_checknumber(L, 4);
        int end_y = (int)luaL_checknumber(L, 5);

        PODVector<vec2> path;
        bool path_found = map->a_star(path, start_x, start_y, end_x, end_y);
        if (!path_found) {
            path.destroy();
            return 0;
        }

        lua_createtable(L, path.size(), 0);
        for (int i = 0; i < path.size(); i++) {
            lua_createtable(L, 0, 2);

            lua_pushnumber(L, path[i].x);
            lua_setfield(L, -2, "x");

            lua_pushnumber(L, path[i].y);
            lua_setfield(L, -2, "y");

            lua_rawseti(L, -2, path.size() - i);
        }

        path.destroy();
        return 1;
    });
    lua_setfield(L, -2, "a_star");

    lua_createtable(L, 0, 1);
    lua_pushcfunction(L, [](lua_State *L) -> int {
        Tilemap *map = (Tilemap *)field_touserdata(L, 1, "udata");
        map->destroy();
        delete map;
        return 0;
    });
    lua_setfield(L, -2, "__gc");
    lua_setmetatable(L, -2);

    return 1;
}

int aux_lib(lua_State *L) {
    const luaL_Reg libs[] = {
        {"make_texture", aux_make_texture},
        {"make_font", aux_make_font},
        {"make_tileset", aux_make_tileset},
        {"make_tilemap", aux_make_tilemap},
        {nullptr, nullptr},
    };

    luaL_newlib(L, libs);
    return 1;
}
