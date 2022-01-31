#include "tilemap.h"
#include "deps/cute_tiled.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

bool Tilemap::try_create(const char *filename, const Tileset &tileset) {
    cute_tiled_map_t *tiled = cute_tiled_load_map_from_file(filename, nullptr);
    if (!tiled) {
        return false;
    }

    // only handling one tileset
    if (!tiled->tilesets || tiled->tilesets->next) {
        return false;
    }

    int firstgid = tiled->tilesets->firstgid;

    for (const cute_tiled_layer_t *layer = tiled->layers; layer;
         layer = layer->next) {
        if (strcmp(layer->type.ptr, "tilelayer") == 0) {
            if (m_collision_map) {
                sg_destroy_buffer(m_vbo);
                delete[] m_collision_map;
                delete[] m_objects;
                return false;
            }

            for (int i = 0; i < tiled->width * tiled->height; i++) {
                if (layer->data[i]) {
                    m_tile_count++;
                }
            }

            m_collision_map =
                new TileCollisionType[tiled->width * tiled->height]{};

            RenVertex *vertices = new RenVertex[m_tile_count * 4]{};

            int vertex_count = 0;
            for (int y = 0; y < tiled->height; y++) {
                for (int x = 0; x < tiled->width; x++) {
                    int index = layer->data[(y * tiled->width) + x];
                    if (index == 0) {
                        continue;
                    }

                    int id = cute_tiled_unset_flags(index) - firstgid;

                    m_collision_map[(y * tiled->width) + x] =
                        tileset.collide_type(id);
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
            if (m_objects) {
                sg_destroy_buffer(m_vbo);
                delete[] m_collision_map;
                delete[] m_objects;
                return false;
            }

            for (cute_tiled_object_t *obj = layer->objects; obj;
                 obj = obj->next) {
                m_object_count++;
            }

            m_objects = new MapObject[m_object_count]{};

            cute_tiled_object_t *obj = layer->objects;
            for (int i = 0; i < m_object_count; i++) {
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
    return true;
}

void Tilemap::destroy() {
    sg_destroy_buffer(m_vbo);
    delete[] m_collision_map;
    delete[] m_objects;
}

bool Tilemap::point_collision(float x, float y) const {
    int ix = (int)x;
    float fx = x - ix;
    int iy = (int)y;
    float fy = y - iy;

    TileCollisionType collision = m_collision_map[iy * m_width + ix];

    switch (collision) {
    case TILE_COLLISION_FULL:
        return true;
    case TILE_COLLISION_OUT_TOP_LEFT:
        return fx <= 0.5 || fy <= 0.5;
    case TILE_COLLISION_OUT_TOP:
        return fy <= 0.5;
    case TILE_COLLISION_OUT_TOP_RIGHT:
        return fx > 0.5 || fy <= 0.5;
    case TILE_COLLISION_OUT_LEFT:
        return fx <= 0.5;
    case TILE_COLLISION_OUT_RIGHT:
        return fx > 0.5;
    case TILE_COLLISION_OUT_BOTTOM_LEFT:
        return fx <= 0.5 || fy > 0.5;
    case TILE_COLLISION_OUT_BOTTOM:
        return fy > 0.5;
    case TILE_COLLISION_OUT_BOTTOM_RIGHT:
        return fx > 0.5 || fy > 0.5;
    case TILE_COLLISION_IN_TOP_LEFT:
        return fx <= 0.5 && fy <= 0.5;
    case TILE_COLLISION_IN_TOP_RIGHT:
        return fx > 0.5 && fy <= 0.5;
    case TILE_COLLISION_IN_BOTTOM_LEFT:
        return fx <= 0.5 && fy > 0.5;
    case TILE_COLLISION_IN_BOTTOM_RIGHT:
        return fx > 0.5 && fy > 0.5;
    default:
        return false;
    }
}

Tilemap::PointMoveResult Tilemap::point_move(vec2 point, vec2 delta,
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

Tilemap::MapObject Tilemap::object_by_type(const char *type) const {
    for (int i = 0; i < m_object_count; i++) {
        if (strcmp(m_objects[i].type, type) == 0) {
            return m_objects[i];
        }
    }

    return {};
}

int Tilemap::objects(MapObject *&out) const {
    out = m_objects;
    return m_object_count;
}

void Tilemap::draw(const Renderer &renderer, RenMatrix mvp,
                   sg_image image) const {
    sg_apply_pipeline(renderer.pip());
    sg_apply_bindings({
        .vertex_buffers = {m_vbo},
        .index_buffer = renderer.quad_ibo(),
        .fs_images = {image},
    });
    sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, SG_RANGE(mvp));
    sg_draw(0, m_tile_count * 6, 1);
}
