#include "tilemap.h"
#include "deps/cute_tiled.h"
#include <string.h>

bool Tilemap::try_create(const char *filename) {
    struct LayerDesc {
        const cute_tiled_tileset_t *tileset;
        const int *data;
        int data_count;
        int map_width;
        int map_height;
        int tile_width;
        int tile_height;
    };

    struct CreateLayerResult {
        sg_buffer vbo;
        int tile_count;
    };

    const auto create_layer = [](const LayerDesc &desc) -> CreateLayerResult {
        const cute_tiled_tileset_t &tileset = *desc.tileset;

        int tile_count = 0;
        for (int i = 0; i < desc.map_width * desc.map_height; i++) {
            if (desc.data[i]) {
                tile_count++;
            }
        }

        RenVertex *vertices = new RenVertex[tile_count * 4]{};

        int vertex_count = 0;
        for (int y = 0; y < desc.map_height; y++) {
            for (int x = 0; x < desc.map_width; x++) {
                int index = desc.data[(y * desc.map_width) + x];
                if (index == 0) {
                    continue;
                }

                int id = cute_tiled_unset_flags(index) - tileset.firstgid;
                float atlas_x = (float)(id % tileset.columns);
                float atlas_y = (float)(id / tileset.columns);

                // bleeding near tile edges
                constexpr float inset = 0.0001f;

                Rect uv = {
                    .x1 =
                        inset + (tileset.margin + (atlas_x * tileset.spacing) +
                                 (atlas_x * desc.tile_width)) /
                                    tileset.imagewidth,
                    .y1 =
                        inset + (tileset.margin + (atlas_y * tileset.spacing) +
                                 (atlas_y * desc.tile_height)) /
                                    tileset.imageheight,
                    .x2 =
                        -inset + (tileset.margin + (atlas_x * tileset.spacing) +
                                  ((atlas_x + 1) * desc.tile_width)) /
                                     tileset.imagewidth,
                    .y2 =
                        -inset + (tileset.margin + (atlas_y * tileset.spacing) +
                                  ((atlas_y + 1) * desc.tile_height)) /
                                     tileset.imageheight,
                };

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

        sg_buffer vbo = sg_make_buffer({
            .type = SG_BUFFERTYPE_VERTEXBUFFER,
            .usage = SG_USAGE_IMMUTABLE,
            .data = {.ptr = vertices, .size = sizeof(RenVertex) * vertex_count},
        });

        delete[] vertices;

        return {
            .vbo = vbo,
            .tile_count = tile_count,
        };
    };

    cute_tiled_map_t *tiled = cute_tiled_load_map_from_file(filename, nullptr);
    if (!tiled) {
        return false;
    }

    if (!tiled->tilesets || tiled->tilesets->next) {
        return false;
    }

    for (const cute_tiled_layer_t *layer = tiled->layers; layer;
         layer = layer->next) {
        if (strcmp(layer->type.ptr, "tilelayer") == 0) {
            CreateLayerResult res = create_layer({
                .tileset = tiled->tilesets,
                .data = layer->data,
                .data_count = layer->data_count,
                .map_width = tiled->width,
                .map_height = tiled->height,
                .tile_width = tiled->tilewidth,
                .tile_height = tiled->tileheight,
            });

            m_vbo = res.vbo;
            m_tile_count = res.tile_count;
        }
    }

    m_width = tiled->width;
    m_height = tiled->height;

    cute_tiled_free_map(tiled);
    return true;
}

void Tilemap::destroy() {}

void Tilemap::draw(const Renderer &renderer, RenMatrix mvp, sg_image image) {
    sg_apply_pipeline(renderer.pip());
    sg_apply_bindings({
        .vertex_buffers = {m_vbo},
        .index_buffer = renderer.quad_ibo(),
        .fs_images = {image},
    });
    sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, SG_RANGE(mvp));
    sg_draw(0, m_tile_count * 6, 1);
}
