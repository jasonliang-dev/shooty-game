#include "api.h"
#include "deps/sokol_app.h"
#include "deps/sokol_gfx.h"
#include "deps/stb_image.h"
#include "font.h"
#include "app.h"
#include <stdio.h>
#include <string.h>

namespace gfx {

static int begin_draw(lua_State *L) {
    sg_begin_default_pass(sg_pass_action{}, sapp_width(), sapp_height());
    app::state->renderer.begin();
    return 0;
}

static int end_draw(lua_State *L) {
    app::state->renderer.flush();
    sg_end_pass();
    sg_commit();
    return 0;
}

static int draw_count(lua_State *L) {
    lua_pushnumber(L, app::state->renderer.draw_count());
    return 1;
}

static int bind_mvp(lua_State *L) {
    Matrix m{};

    lua_pushnil(L);
    for (int i = 0; lua_next(L, 1); i++) {
        m.arr[i] = (float)luaL_checknumber(L, -1);
        lua_pop(L, 1);
    }

    app::state->renderer.mvp(m);
    return 0;
}

static int bind_white_texture(lua_State *L) {
    (void)L;
    app::state->renderer.texture(app::state->white);
    return 0;
}

static int bind_texture(lua_State *L) {
    u32 id = (u32)luaL_checkinteger(L, 1);
    app::state->renderer.texture({id});
    return 0;
}

static int v3_t2(lua_State *L) {
    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    float z = (float)luaL_checknumber(L, 3);
    float u = (float)luaL_checknumber(L, 4);
    float v = (float)luaL_checknumber(L, 5);

    app::state->renderer.push({
        .pos = {x, y, z},
        .uv = {u, v},
        .color = {255, 255, 255, 255},
        .fog = {0, 0, 0, 0},
    });

    return 0;
}

static int v3_t2_c4_f4(lua_State *L) {
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

    app::state->renderer.push({
        .pos = {x, y, z},
        .uv = {u, v},
        .color = {r, g, b, a},
        .fog = {fr, fg, fb, fa},
    });

    return 0;
}

static int make_texture(lua_State *L) {
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
        u32 id = (u32)luax::field_checkinteger(L, 1, "id");
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

    lua_pushlightuserdata(L, new Font(filename, size));
    lua_setfield(L, -2, "udata");

    lua_pushcfunction(L, [](lua_State *L) -> int {
        Font *font = (Font *)luax::field_touserdata(L, 1, "udata");

        const char *text = luaL_checkstring(L, 2);
        float x = (float)luaL_checknumber(L, 3);
        float y = (float)luaL_checknumber(L, 4);

        // can't brace initialize?
        Font::PrintDesc desc{};
        desc.text = text;
        desc.x = x;
        desc.y = y;
        memset(desc.color, 255, sizeof(u8) * 4);
        desc.alignment = FontAlign::left | FontAlign::bottom;

        float n = font->print(app::state->renderer, desc);

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

static int make_vertex_array(lua_State *L) {
    int count = (int)luaL_checkinteger(L, 1);

    lua_createtable(L, 0, 4);

    lua_pushlightuserdata(L, new Vertex[count]{});
    lua_setfield(L, -2, "udata");

    lua_pushcfunction(L, [](lua_State *L) -> int {
        Vertex *vertices = (Vertex *)luax::field_touserdata(L, 1, "udata");
        int index = (int)luaL_checkinteger(L, 2);

        float x = (float)luaL_checknumber(L, 3);
        float y = (float)luaL_checknumber(L, 4);
        float z = (float)luaL_checknumber(L, 5);
        float u = (float)luaL_optnumber(L, 6, 0);
        float v = (float)luaL_optnumber(L, 7, 0);
        u8 r = (u8)luaL_optinteger(L, 8, 255);
        u8 g = (u8)luaL_optinteger(L, 9, 255);
        u8 b = (u8)luaL_optinteger(L, 10, 255);
        u8 a = (u8)luaL_optinteger(L, 11, 255);
        u8 fr = (u8)luaL_optinteger(L, 12, 0);
        u8 fg = (u8)luaL_optinteger(L, 13, 0);
        u8 fb = (u8)luaL_optinteger(L, 14, 0);
        u8 fa = (u8)luaL_optinteger(L, 15, 0);

        vertices[index - 1].pos[0] = x;
        vertices[index - 1].pos[1] = y;
        vertices[index - 1].pos[2] = z;
        vertices[index - 1].uv[0] = u;
        vertices[index - 1].uv[1] = v;
        vertices[index - 1].color[0] = r;
        vertices[index - 1].color[1] = g;
        vertices[index - 1].color[2] = b;
        vertices[index - 1].color[3] = a;
        vertices[index - 1].fog[0] = fr;
        vertices[index - 1].fog[1] = fg;
        vertices[index - 1].fog[2] = fb;
        vertices[index - 1].fog[3] = fa;

        return 0;
    });
    lua_setfield(L, -2, "write_at");

    lua_pushcfunction(L, [](lua_State *L) -> int {
        Vertex *vertices = (Vertex *)luax::field_touserdata(L, 1, "udata");
        int length = (int)luaL_checkinteger(L, 2);
        app::state->renderer.push(vertices, length);
        return 0;
    });
    lua_setfield(L, -2, "draw");

    lua_createtable(L, 0, 1);
    lua_pushcfunction(L, [](lua_State *L) -> int {
        Vertex *vertices = (Vertex *)luax::field_touserdata(L, 1, "udata");
        delete[] vertices;
        return 0;
    });
    lua_setfield(L, -2, "__gc");
    lua_setmetatable(L, -2);

    return 1;
}

int lib(lua_State *L) {
    const luaL_Reg libs[] = {
        {"begin_draw", begin_draw},
        {"end_draw", end_draw},
        {"draw_count", draw_count},
        {"bind_mvp", bind_mvp},
        {"bind_white_texture", bind_white_texture},
        {"bind_texture", bind_texture},
        {"v3_t2", v3_t2},
        {"v3_t2_c4_f4", v3_t2_c4_f4},
        {"make_texture", make_texture},
        {"make_font", make_font},
        {"make_vertex_array", make_vertex_array},
        {nullptr, nullptr},
    };

    luaL_newlib(L, libs);
    return 1;
}

} // namespace gfx
