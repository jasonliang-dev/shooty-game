#pragma once

#include "pod_vector.h"
#include "priority_queue.h"
#include "renderer.h"
#include "tileset.h"

struct TilemapPointMoveResult {
    float x;
    float y;
    bool collided;
};

struct TilemapObject {
    char type[64];
    float x;
    float y;
};

struct Tilemap {
    const char *try_create(const char *filename, const Tileset &tileset);
    void destroy();

    bool point_collision(float x, float y) const;
    TilemapPointMoveResult point_move(vec2 point, vec2 delta,
                                      int sub_steps) const;

    TilemapObject object_by_type(const char *type) const;
    int objects(TilemapObject *&out) const;

    bool a_star(PODVector<vec2> &out, int start_x, int start_y, int end_x,
                int end_y);

    void draw(const Renderer &renderer, RenMatrix mvp, sg_image image) const;

  private:
    sg_buffer m_vbo{};

    TileCollisionType *m_collision_map = nullptr;

    TilemapObject *m_objects = nullptr;
    int m_object_count = 0;

    struct GraphNode {
        int x;
        int y;
        float tile_cost;
        float f_cost;
        float g_cost;
        bool invalid;

        // index to m_keep_alive
        int parent;
    };

    GraphNode *m_graph = nullptr;
    int *m_alive_indices = nullptr;
    GraphNode *m_keep_alive = nullptr;
    PriorityQueue<int> m_frontier;

    int m_width = 0;
    int m_height = 0;
    int m_tile_count = 0;
};
