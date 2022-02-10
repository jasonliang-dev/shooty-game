#include "water_renderer.h"
#include "shd_water.h"

void WaterRenderer::create() {
    using vec3 = float[3];
    vec3 vertices[6] = {
        {0, 0, 0}, {0, 1, 0}, {1, 1, 0}, {0, 0, 0}, {1, 1, 0}, {1, 0, 0},
    };
    m_vbo = sg_make_buffer({
        .type = SG_BUFFERTYPE_VERTEXBUFFER,
        .usage = SG_USAGE_IMMUTABLE,
        .data = SG_RANGE(vertices),
    });

    sg_shader shd_water =
        sg_make_shader(shd_water_shader_desc(sg_query_backend()));

    sg_pipeline_desc desc{};
    desc.shader = shd_water;
    desc.layout.attrs[ATTR_vert_a_position].format = SG_VERTEXFORMAT_FLOAT3;
    desc.layout.attrs[ATTR_vert_a_position].buffer_index = 0;
    desc.depth.compare = SG_COMPAREFUNC_LESS_EQUAL;
    desc.depth.write_enabled = true;

    m_pip = sg_make_pipeline(desc);
}

void WaterRenderer::destroy() {
    sg_destroy_pipeline(m_pip);
    sg_destroy_buffer(m_vbo);
}

void WaterRenderer::draw(Matrix mvp, float scale, float time) {
    sg_apply_pipeline(m_pip);
    sg_apply_bindings({.vertex_buffers = {m_vbo}});
    vs_uniforms_water_t vs_uniforms = {.u_mvp = mvp};
    sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_uniforms_water,
                      SG_RANGE(vs_uniforms));
    fs_uniforms_water_t fs_uniforms = {
        .u_scale = scale,
        .u_time = time,
    };
    sg_apply_uniforms(SG_SHADERSTAGE_FS, SLOT_fs_uniforms_water,
                      SG_RANGE(fs_uniforms));
    sg_draw(0, 6, 1);
}
