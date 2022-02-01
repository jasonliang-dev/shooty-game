#pragma once

#include "pod_vector.h"
#include "priority_queue.h"
#include "renderer.h"
#include "tileset.h"

struct Tilemap {
    const char *try_create(const char *filename, const Tileset &tileset);
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

    bool a_star(PODVector<vec2> &out, int start_x, int start_y, int end_x,
                int end_y);
    void draw(const Renderer &renderer, RenMatrix mvp, sg_image image) const;

  private:
    sg_buffer m_vbo{};

    TileCollisionType *m_collision_map = nullptr;

    MapObject *m_objects = nullptr;
    int m_object_count = 0;

    struct GraphNode {
        int x;
        int y;
        float tile_cost;
        float f_cost;
        float g_cost;
        bool visited;
        bool invalid;

        // index to m_keep_alive
        // can't do `GraphNode *parent` since
        // m_keep_alive.push_back() can invalidate the pointer
        int parent;
    };

    GraphNode *m_graph = nullptr;
    PriorityQueue<int> m_frontier;
    PODVector<GraphNode> m_keep_alive;

    int m_width = 0;
    int m_height = 0;
    int m_tile_count = 0;
};
