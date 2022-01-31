#pragma once

#include "tileset.h"
#include "renderer.h"

struct Tilemap {
    bool try_create(const char *filename, const Tileset &tileset);
    void destroy();

    bool point_collision(float x, float y) const;

    struct PointMoveResult {
        float x;
        float y;
        bool collided;
    };
    PointMoveResult point_move(vec2 point, vec2 delta, int sub_steps) const;

    struct MapObject {
        char type[64];
        float x;
        float y;
    };
    MapObject object_by_type(const char *type) const;
    int objects(MapObject *&out) const;

    void a_star(vec2 start, vec2 end) const;
    void draw(const Renderer &renderer, RenMatrix mvp, sg_image image) const;

  private:
    sg_buffer m_vbo{};
    TileCollisionType *m_collision_map = nullptr;
    MapObject *m_objects = nullptr;
    int m_object_count = 0;
    int m_width = 0;
    int m_height = 0;
    int m_tile_count = 0;
};
