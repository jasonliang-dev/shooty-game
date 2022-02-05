#define _CRT_SECURE_NO_WARNINGS

#include "tilemap.h"
#include "deps/cute_tiled.h"
#include "shd_default.h"
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

const char *Tilemap::try_create(const char *filename, const Tileset &tileset) {
    cute_tiled_map_t *tiled = cute_tiled_load_map_from_file(filename, nullptr);
    if (!tiled) {
        return "failed loading tilemap";
    }

    if (!tiled->tilesets || tiled->tilesets->next) {
        return "tilemap should only have one tileset";
    }

    int firstgid = tiled->tilesets->firstgid;

    for (const cute_tiled_layer_t *layer = tiled->layers; layer;
         layer = layer->next) {
        if (strcmp(layer->type.ptr, "tilelayer") == 0) {
            if (m_collision_map) {
                destroy();
                return "tilemap should only have one tile layer";
            }

            for (int i = 0; i < tiled->width * tiled->height; i++) {
                if (layer->data[i]) {
                    m_tile_count++;
                }
            }

            m_collision_map =
                new TileCollisionType[tiled->width * tiled->height]{};

            m_graph = new GraphNode[tiled->width * tiled->height]{};
            m_alive_indices = new int[tiled->width * tiled->height]{};
            m_keep_alive = new GraphNode[tiled->width * tiled->height]{};

            RenVertex *vertices = new RenVertex[m_tile_count * 4]{};

            int vertex_count = 0;
            for (int y = 0; y < tiled->height; y++) {
                for (int x = 0; x < tiled->width; x++) {
                    int index = layer->data[(y * tiled->width) + x];
                    if (index == 0) {
                        continue;
                    }

                    int id = cute_tiled_unset_flags(index) - firstgid;

                    TileCollisionType collide_type = tileset.collide_type(id);
                    m_collision_map[y * tiled->width + x] = collide_type;

                    m_graph[y * tiled->width + x].x = x;
                    m_graph[y * tiled->width + x].y = y;

                    switch (collide_type) {
                    case TILE_COLLISION_FULL:
                        m_graph[y * tiled->width + x].tile_cost = -1;
                        break;
                    case TILE_COLLISION_NONE:
                        m_graph[y * tiled->width + x].tile_cost = 1;
                        break;
                    default:
                        m_graph[y * tiled->width + x].tile_cost = 10;
                        break;
                    }

                    Rect uv = tileset.uv(id);

                    vertices[vertex_count + 0] = {
                        .pos = {(float)x + 0, (float)y + 0, 0},
                        .uv = {uv.x1, uv.y1},
                        .color = {255, 255, 255, 255},
                        .fog = {0, 0, 0, 0},
                    };

                    vertices[vertex_count + 1] = {
                        .pos = {(float)x + 0, (float)y + 1, 0},
                        .uv = {uv.x1, uv.y2},
                        .color = {255, 255, 255, 255},
                        .fog = {0, 0, 0, 0},
                    };

                    vertices[vertex_count + 2] = {
                        .pos = {(float)x + 1, (float)y + 1, 0},
                        .uv = {uv.x2, uv.y2},
                        .color = {255, 255, 255, 255},
                        .fog = {0, 0, 0, 0},
                    };

                    vertices[vertex_count + 3] = {
                        .pos = {(float)x + 1, (float)y + 0, 0},
                        .uv = {uv.x2, uv.y1},
                        .color = {255, 255, 255, 255},
                        .fog = {0, 0, 0, 0},
                    };

                    vertex_count += 4;
                }
            }

            m_vbo = sg_make_buffer({
                .type = SG_BUFFERTYPE_VERTEXBUFFER,
                .usage = SG_USAGE_IMMUTABLE,
                .data = {.ptr = vertices,
                         .size = sizeof(RenVertex) * vertex_count},
            });

            delete[] vertices;
        } else if (strcmp(layer->type.ptr, "objectgroup") == 0) {
            int old_count = m_object_count;
            for (cute_tiled_object_t *obj = layer->objects; obj;
                 obj = obj->next) {
                m_object_count++;
            }

            TilemapObject *objects = new TilemapObject[m_object_count]{};
            memcpy(objects, m_objects, sizeof(TilemapObject) * old_count);
            delete[] m_objects;
            m_objects = objects;

            cute_tiled_object_t *obj = layer->objects;
            for (int i = old_count; i < m_object_count; i++) {
                m_objects[i] = {
                    .x = obj->x / tileset.tile_width(),
                    .y = obj->y / tileset.tile_height(),
                };
                strncpy(m_objects[i].type, obj->type.ptr,
                        array_count(m_objects[i].type));
                obj = obj->next;
            }
        }
    }

    m_width = tiled->width;
    m_height = tiled->height;

    cute_tiled_free_map(tiled);
    return nullptr;
}

void Tilemap::destroy() {
    sg_destroy_buffer(m_vbo);

    delete[] m_collision_map;
    delete[] m_objects;

    delete[] m_graph;
    delete[] m_alive_indices;
    delete[] m_keep_alive;
    m_frontier.destroy();
}

bool Tilemap::point_collision(float x, float y) const {
    int ix = (int)x;
    float fx = x - ix;
    int iy = (int)y;
    float fy = y - iy;

    TileCollisionType collision = m_collision_map[iy * m_width + ix];

    constexpr float pad = 0.75f;
    constexpr float dap = 1 - pad;
    switch (collision) {
    case TILE_COLLISION_FULL:
        return true;
    case TILE_COLLISION_OUT_TOP_LEFT:
        return fx <= dap || fy <= dap;
    case TILE_COLLISION_OUT_TOP:
        return fy <= dap;
    case TILE_COLLISION_OUT_TOP_RIGHT:
        return fx > pad || fy <= dap;
    case TILE_COLLISION_OUT_LEFT:
        return fx <= dap;
    case TILE_COLLISION_OUT_RIGHT:
        return fx > pad;
    case TILE_COLLISION_OUT_BOTTOM_LEFT:
        return fx <= dap || fy > pad;
    case TILE_COLLISION_OUT_BOTTOM:
        return fy > pad;
    case TILE_COLLISION_OUT_BOTTOM_RIGHT:
        return fx > pad || fy > pad;
    case TILE_COLLISION_IN_TOP_LEFT:
        return fx <= dap && fy <= dap;
    case TILE_COLLISION_IN_TOP_RIGHT:
        return fx > pad && fy <= dap;
    case TILE_COLLISION_IN_BOTTOM_LEFT:
        return fx <= dap && fy > pad;
    case TILE_COLLISION_IN_BOTTOM_RIGHT:
        return fx > pad && fy > pad;
    default:
        return false;
    }
}

TilemapPointMoveResult Tilemap::point_move(vec2 point, vec2 delta,
                                           int sub_steps) const {
    float dx = delta.x / sub_steps;
    float dy = delta.y / sub_steps;

    bool collided = false;

    float x = point.x;
    float y = point.y;
    for (int i = 0; i < sub_steps; i++) {
        float tx = x;
        x += dx;
        if (point_collision(x, y)) {
            x = tx;
            collided = true;
        }

        float ty = y;
        y += dy;
        if (point_collision(x, y)) {
            y = ty;
            collided = true;
        }
    }

    return {
        .x = x,
        .y = y,
        .collided = collided,
    };
}

TilemapObject Tilemap::object_by_type(const char *type) const {
    for (int i = 0; i < m_object_count; i++) {
        if (strcmp(m_objects[i].type, type) == 0) {
            return m_objects[i];
        }
    }

    return {};
}

int Tilemap::objects(TilemapObject *&out) const {
    out = m_objects;
    return m_object_count;
}

bool Tilemap::a_star(PODVector<vec2> &out, int start_x, int start_y, int end_x,
                     int end_y) {
    const auto distance = [](int ax, int ay, int bx, int by) {
        float dx = (float)abs(ax - bx);
        float dy = (float)abs(ay - by);
        return sqrtf(dx * dx + dy * dy);
    };

    m_frontier.clear();

    for (int i = 0; i < m_width * m_height; i++) {
        m_graph[i].f_cost = 0;
        m_graph[i].g_cost = FLT_MAX;
        m_graph[i].parent = -1;

        m_alive_indices[i] = -1;
    }

    if (start_x == end_x && start_y == end_y) {
        out.push_back(vec2((float)start_x, (float)start_y));
        return true;
    }

    GraphNode *goal = &m_graph[end_y * m_width + end_x];
    if (goal->tile_cost < 0) {
        return false;
    }

    GraphNode *begin = &m_graph[start_y * m_width + start_x];
    if (begin->tile_cost < 0) {
        return false;
    }

    int alive_count = 0;

    begin->g_cost = 0;
    begin->f_cost = distance(start_x, start_y, end_x, end_y);
    m_keep_alive[alive_count] = *begin;
    m_frontier.push_min(alive_count, begin->f_cost);
    m_alive_indices[start_y * m_width + start_x] = alive_count;
    alive_count++;

    while (m_frontier.size()) {
        int top_ptr = m_frontier.pop_min();
        GraphNode *top = &m_keep_alive[top_ptr];
        if (top->invalid) {
            continue;
        }

        if (top->x == end_x && top->y == end_y) {
            while (top_ptr != -1) {
                GraphNode *node = &m_keep_alive[top_ptr];
                out.push_back(vec2((float)node->x, (float)node->y));
                top_ptr = node->parent;
            }

            return true;
        }

        int neighbor_count = 0;
        GraphNode *neighbors[8];

        for (int y = top->y - 1; y <= top->y + 1; y++) {
            for (int x = top->x - 1; x <= top->x + 1; x++) {
                if (x == top->x && y == top->y) {
                    continue;
                }

                if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
                    continue;
                }

                GraphNode *node = &m_graph[y * m_width + x];
                if (node->tile_cost < 0) {
                    continue;
                }

                neighbors[neighbor_count++] = node;
            }
        }

        for (int i = 0; i < neighbor_count; i++) {
            GraphNode *neighbor = neighbors[i];

            float tentative =
                top->g_cost + neighbor->tile_cost +
                distance(neighbor->x, neighbor->y, top->x, top->y);

            if (tentative < neighbor->g_cost) {
                neighbor->g_cost = tentative;
                neighbor->f_cost =
                    neighbor->g_cost +
                    distance(neighbor->x, neighbor->y, end_x, end_y);
                neighbor->parent = top_ptr;

                int live = m_alive_indices[neighbor->y * m_width + neighbor->x];
                if (live != -1) {
                    m_keep_alive[live].invalid = true;
                }

                m_keep_alive[alive_count] = *neighbor;
                m_frontier.push_min(alive_count, neighbor->f_cost);
                m_alive_indices[neighbor->y * m_width + neighbor->x] =
                    alive_count;
                alive_count++;
            }
        }
    }

    return false;
}

void Tilemap::draw(const Renderer &renderer, RenMatrix mvp,
                   sg_image image) const {
    sg_apply_pipeline(renderer.pip());
    sg_apply_bindings({
        .vertex_buffers = {m_vbo},
        .index_buffer = renderer.quad_ibo(),
        .fs_images = {image},
    });
    vs_params_t uniforms = {.u_mvp = mvp};
    sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, SG_RANGE(uniforms));
    sg_draw(0, m_tile_count * 6, 1);
}
