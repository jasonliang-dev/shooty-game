#pragma once

#include "tileset.h"
#include "renderer.h"

struct Tilemap {
    struct MapObject {
        char *type;
        float x;
        float y;
    };

    bool try_create(const char *filename, const Tileset *tileset);
    void destroy();

    bool point_collision(float x, float y);

    struct PointMoveResult {
        float x;
        float y;
        bool collided;
    };
    PointMoveResult point_move(vec2 point, vec2 delta, int sub_steps);

    void a_star(vec2 start, vec2 end);
    void draw(const Renderer &renderer, RenMatrix mvp, sg_image image);

  private:
    sg_buffer m_vbo{};
    TileCollisionType *m_collision_map = nullptr;
    int m_width = 0;
    int m_height = 0;
    int m_tile_count = 0;
};
