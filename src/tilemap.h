#pragma once

#include "deps/sokol_gfx.h"
#include "renderer.h"

struct Tilemap {
    struct MapObject {
        char *type;
        float x;
        float y;
    };

    bool try_create(const char *filename);
    void destroy();

    bool point_collision(vec2 point);

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

    u8 *m_collision = nullptr;
    int m_width = 0;
    int m_height = 0;
    int m_tile_count = 0;
};
