#include "renderer.h"
#include "shd_default.h"
#include <stdio.h>
#include <string.h>

void Renderer::create(int vertex_capacity) {
    m_vbo = sg_make_buffer({
        .size = sizeof(RenVertex) * vertex_capacity,
        .type = SG_BUFFERTYPE_VERTEXBUFFER,
        .usage = SG_USAGE_STREAM,
    });

    u16 *indices = new u16[vertex_capacity * 6 / 4]{};
    u16 stride = 0;
    for (int i = 0; i < vertex_capacity * 6 / 4; i += 6) {
        indices[i + 0] = stride + 0;
        indices[i + 1] = stride + 1;
        indices[i + 2] = stride + 2;

        indices[i + 3] = stride + 0;
        indices[i + 4] = stride + 2;
        indices[i + 5] = stride + 3;

        stride += 4;
    }

    m_ibo = sg_make_buffer({
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .usage = SG_USAGE_IMMUTABLE,
        .data = {.ptr = indices, .size = sizeof(u16) * vertex_capacity * 6 / 4},
    });

    delete[] indices;

    sg_shader shd_default =
        sg_make_shader(shd_default_shader_desc(sg_query_backend()));

    sg_pipeline_desc pdesc{};
    pdesc.shader = shd_default;
    pdesc.layout.attrs[ATTR_vert_a_position].format = SG_VERTEXFORMAT_FLOAT3;
    pdesc.layout.attrs[ATTR_vert_a_position].buffer_index = 0;
    pdesc.layout.attrs[ATTR_vert_a_texindex].format = SG_VERTEXFORMAT_FLOAT2;
    pdesc.layout.attrs[ATTR_vert_a_texindex].buffer_index = 0;
    pdesc.layout.attrs[ATTR_vert_a_color].format = SG_VERTEXFORMAT_UBYTE4N;
    pdesc.layout.attrs[ATTR_vert_a_color].buffer_index = 0;
    pdesc.layout.attrs[ATTR_vert_a_fog].format = SG_VERTEXFORMAT_UBYTE4N;
    pdesc.layout.attrs[ATTR_vert_a_fog].buffer_index = 0;
    pdesc.index_type = SG_INDEXTYPE_UINT16;
    pdesc.depth.compare = SG_COMPAREFUNC_LESS_EQUAL;
    pdesc.depth.write_enabled = true;
    pdesc.colors[0].blend.enabled = true;
    pdesc.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
    pdesc.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

    m_pip = sg_make_pipeline(pdesc);

    m_vertices = new RenVertex[vertex_capacity]{};
    m_vertex_count = 0;
    m_vertex_capacity = vertex_capacity;
}

void Renderer::destroy() {
    sg_destroy_pipeline(m_pip);
    sg_destroy_buffer(m_vbo);
    sg_destroy_buffer(m_ibo);
    delete m_vertices;
}

void Renderer::begin() { m_draw_count = 0; }

void Renderer::flush() {
    if (m_vertex_count == 0) {
        return;
    }
    // printf("vertex_count: %d\n", m_vertex_count);
    // printf("vertex_capacity: %d\n", m_vertex_capacity);

    int off = sg_append_buffer(
        m_vbo, {.ptr = m_vertices, .size = sizeof(RenVertex) * m_vertex_count});

    sg_apply_pipeline(m_pip);
    sg_apply_bindings({
        .vertex_buffers = {m_vbo},
        .vertex_buffer_offsets = {off},
        .index_buffer = m_ibo,
        .fs_images = {u_texture},
    });
    vs_params_t uniforms = {.u_mvp = u_mvp};
    sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, SG_RANGE(uniforms));
    sg_draw(0, m_vertex_count * 6 / 4, 1);

    m_vertex_count = 0;
    m_draw_count++;
}

void Renderer::push(RenVertex vertex) {
    if (m_vertex_count == m_vertex_capacity) {
        flush();
    }

    m_vertices[m_vertex_count++] = vertex;
}

void Renderer::push(RenVertex *vertices, int n) {
    if (m_vertex_count + n > m_vertex_capacity) {
        flush();
    }

    memcpy(m_vertices + m_vertex_count, vertices, sizeof(RenVertex) * n);
    m_vertex_count += n;
}

void Renderer::mvp(RenMatrix mat) {
    flush();
    u_mvp = mat;
}

void Renderer::texture(sg_image tex) {
    if (u_texture.id != tex.id) {
        flush();
        u_texture = tex;
    }
}

int Renderer::draw_count() const { return m_draw_count; }
sg_pipeline Renderer::pip() const { return m_pip; }
sg_buffer Renderer::quad_ibo() const { return m_ibo; }
RenMatrix Renderer::mvp() const { return u_mvp; }
sg_image Renderer::texture() const { return u_texture; }
