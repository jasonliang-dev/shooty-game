#include "api.h"
#include "app.h"
#include "deps/sokol_app.h"
#include "deps/sokol_gfx.h"

static int gfx_begin_draw(lua_State *L) {
    lua_getfield(L, 1, "color");

    const auto geti_checknumber = [](lua_State *L, int index, int n) {
        lua_rawgeti(L, index, n);
        float num = (float)luaL_checknumber(L, -1);
        lua_pop(L, 1);
        return num;
    };

    float r = geti_checknumber(L, -1, 1) / 255;
    float g = geti_checknumber(L, -1, 2) / 255;
    float b = geti_checknumber(L, -1, 3) / 255;
    float a = geti_checknumber(L, -1, 4) / 255;

    lua_pop(L, 1);

    sg_pass_action act{};
    act.colors[0].action = SG_ACTION_CLEAR;
    act.colors[0].value = {r, g, b, a};
    sg_begin_default_pass(act, sapp_width(), sapp_height());
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

    for (int i = 0; i < 16; i++) {
        lua_rawgeti(L, 1, i + 1);
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

static int gfx_v3_t2_f4(lua_State *L) {
    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    float z = (float)luaL_checknumber(L, 3);
    float u = (float)luaL_optnumber(L, 4, 0);
    float v = (float)luaL_optnumber(L, 5, 0);
    u8 fr = (u8)luaL_optinteger(L, 6, 0);
    u8 fg = (u8)luaL_optinteger(L, 7, 0);
    u8 fb = (u8)luaL_optinteger(L, 8, 0);
    u8 fa = (u8)luaL_optinteger(L, 9, 0);

    app->renderer.push({
        .pos = {x, y, z},
        .uv = {u, v},
        .color = {255, 255, 255, 255},
        .fog = {fr, fg, fb, fa},
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
        {"v3_t2_f4", gfx_v3_t2_f4},
        {"v3_t2_c4_f4", gfx_v3_t2_c4_f4},
        {nullptr, nullptr},
    };

    luaL_newlib(L, libs);
    return 1;
}
