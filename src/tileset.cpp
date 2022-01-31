#include "tileset.h"
#include "deps/cute_tiled.h"
#include "texture.h"
#include <stdio.h>
#include <string.h>

bool Tileset::try_create(const char *filename) {
    cute_tiled_tileset_t *tiled =
        cute_tiled_load_external_tileset(filename, nullptr);

    char image_path[2048]{};
    const char *end = strrchr(filename, '/');
    int len = (int)(end - filename) + 1;
    memcpy(image_path, filename, len);
    strncpy(image_path + len, tiled->image.ptr, array_count(image_path) - len);

    m_texture.try_create(image_path);

    m_collide = new TileCollisionType[tiled->tilecount]{};

    for (const cute_tiled_tile_descriptor_t *tile = tiled->tiles; tile;
         tile = tile->next) {
        if (strcmp(tile->type.ptr, "full") == 0) {
            m_collide[tile->tile_index] = TILE_COLLISION_FULL;
        } else if (strcmp(tile->type.ptr, "out top left") == 0) {
            m_collide[tile->tile_index] = TILE_COLLISION_OUT_TOP_LEFT;
        } else if (strcmp(tile->type.ptr, "out top") == 0) {
            m_collide[tile->tile_index] = TILE_COLLISION_OUT_TOP;
        } else if (strcmp(tile->type.ptr, "out top right") == 0) {
            m_collide[tile->tile_index] = TILE_COLLISION_OUT_TOP_RIGHT;
        } else if (strcmp(tile->type.ptr, "out left") == 0) {
            m_collide[tile->tile_index] = TILE_COLLISION_OUT_LEFT;
        } else if (strcmp(tile->type.ptr, "out right") == 0) {
            m_collide[tile->tile_index] = TILE_COLLISION_OUT_RIGHT;
        } else if (strcmp(tile->type.ptr, "out bottom left") == 0) {
            m_collide[tile->tile_index] = TILE_COLLISION_OUT_BOTTOM_LEFT;
        } else if (strcmp(tile->type.ptr, "out bottom") == 0) {
            m_collide[tile->tile_index] = TILE_COLLISION_OUT_BOTTOM;
        } else if (strcmp(tile->type.ptr, "out bottom right") == 0) {
            m_collide[tile->tile_index] = TILE_COLLISION_OUT_BOTTOM_RIGHT;
        } else if (strcmp(tile->type.ptr, "in bottom right") == 0) {
            m_collide[tile->tile_index] = TILE_COLLISION_IN_BOTTOM_RIGHT;
        } else if (strcmp(tile->type.ptr, "in bottom left") == 0) {
            m_collide[tile->tile_index] = TILE_COLLISION_IN_BOTTOM_LEFT;
        } else if (strcmp(tile->type.ptr, "in top left") == 0) {
            m_collide[tile->tile_index] = TILE_COLLISION_IN_TOP_LEFT;
        } else if (strcmp(tile->type.ptr, "in top right") == 0) {
            m_collide[tile->tile_index] = TILE_COLLISION_IN_TOP_RIGHT;
        }
    }

    m_columns = tiled->columns;
    m_margin = tiled->margin;
    m_spacing = tiled->spacing;
    m_tile_width = tiled->tilewidth;
    m_tile_height = tiled->tileheight;

    cute_tiled_free_external_tileset(tiled);
    return true;
}

void Tileset::destroy() {
    m_texture.destroy();
    delete[] m_collide;
}

Rect Tileset::uv(int id) const {
    constexpr float inset = 0.006f;

    int m = m_margin;
    int p = m_spacing;
    int tw = m_tile_width;
    int th = m_tile_height;
    int aw = m_texture.m_width;
    int ah = m_texture.m_height;

    // atlas coords
    float ax = (float)(id % m_columns);
    float ay = (float)(id / m_columns);

    return {
        .x1 = (m + ax * p + ax * tw) / aw + inset,
        .y1 = (m + ay * p + ay * th) / ah + inset,
        .x2 = (m + ax * p + (ax + 1) * tw) / aw - inset,
        .y2 = (m + ay * p + (ay + 1) * th) / ah - inset,
    };
}

TileCollisionType Tileset::collide_type(int id) const { return m_collide[id]; }
sg_image Tileset::image() const { return m_texture.m_image; }
int Tileset::tile_width() const { return m_tile_width; }
int Tileset::tile_height() const { return m_tile_height; }
