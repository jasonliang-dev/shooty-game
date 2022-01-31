#pragma once

#include "deps/sokol_gfx.h"
#include "texture.h"
#include "types.h"

enum TileCollisionType : u8 {
    TILE_COLLISION_NONE,
    TILE_COLLISION_FULL,
    TILE_COLLISION_OUT_TOP_LEFT,
    TILE_COLLISION_OUT_TOP,
    TILE_COLLISION_OUT_TOP_RIGHT,
    TILE_COLLISION_OUT_LEFT,
    TILE_COLLISION_OUT_RIGHT,
    TILE_COLLISION_OUT_BOTTOM_LEFT,
    TILE_COLLISION_OUT_BOTTOM,
    TILE_COLLISION_OUT_BOTTOM_RIGHT,
    TILE_COLLISION_IN_BOTTOM_RIGHT,
    TILE_COLLISION_IN_BOTTOM_LEFT,
    TILE_COLLISION_IN_TOP_RIGHT,
    TILE_COLLISION_IN_TOP_LEFT,
};

struct Tileset {
    bool try_create(const char *filename);
    void destroy();

    Rect uv(int id) const;
    TileCollisionType collide_type(int id) const;
    sg_image image() const;

  private:
    Texture m_texture{};
    TileCollisionType *m_collide = nullptr;
    int m_columns = 0;
    int m_margin = 0;
    int m_spacing = 0;
    int m_tile_width = 0;
    int m_tile_height = 0;
};
