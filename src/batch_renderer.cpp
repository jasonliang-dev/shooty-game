#include "batch_renderer.h"

BatchRenderer::BatchRenderer(int vertex_capacity) {
    m_vbo = sg_make_buffer({
        .size = sizeof(Vertex) * vertex_capacity,
        .type = SG_BUFFERTYPE_VERTEXBUFFER,
        .usage = SG_USAGE_STREAM,
    });

    unsigned short *indices = new unsigned short[vertex_capacity * 6 / 4];
    unsigned short stride = 0;
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
        .data = {.ptr = indices,
                 .size = sizeof(unsigned short) * vertex_capacity * 6 / 4},
    });

    delete[] indices;

    sg_shader_desc sdesc{};

    sdesc.vs.uniform_blocks[0].size = sizeof(Matrix);
    sdesc.vs.uniform_blocks[0].uniforms[0].name = "u_mvp";
    sdesc.vs.uniform_blocks[0].uniforms[0].type = SG_UNIFORMTYPE_MAT4;
    sdesc.fs.images[0].name = "u_texture";
    sdesc.fs.images[0].image_type = SG_IMAGETYPE_2D;
    sdesc.fs.images[0].sampler_type = SG_SAMPLERTYPE_FLOAT;

    sdesc.vs.source = "#version 330 core\n"

                      "layout(location=0) in vec3 a_position;\n"
                      "layout(location=1) in vec2 a_texindex;\n"
                      "layout(location=2) in vec4 a_color;\n"
                      "layout(location=3) in vec4 a_fog;\n"
                      "out vec2 v_texindex;\n"
                      "out vec4 v_color;\n"
                      "out vec4 v_fog;\n"
                      "uniform mat4 u_mvp;\n"

                      "void main() {\n"
                      "    v_texindex = a_texindex;\n"
                      "    v_color = a_color;\n"
                      "    v_fog = a_fog;\n"
                      "    gl_Position = u_mvp * vec4(a_position, 1);\n"
                      "}\n";

    sdesc.fs.source =
        "#version 330 core\n"

        "in vec2 v_texindex;\n"
        "in vec4 v_color;\n"
        "in vec4 v_fog;\n"
        "out vec4 f_color;\n"
        "uniform sampler2D u_texture;\n"

        "void main() {\n"
        "    vec4 result = texture(u_texture, v_texindex) * v_color;\n"
        "    if (result.a < 0.1) discard;\n"
        "    f_color = result + v_fog;\n"
        "}\n";

    sg_pipeline_desc pdesc{};
    pdesc.shader = sg_make_shader(sdesc);
    pdesc.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;
    pdesc.layout.attrs[0].buffer_index = 0;
    pdesc.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT2;
    pdesc.layout.attrs[1].buffer_index = 0;
    pdesc.layout.attrs[2].format = SG_VERTEXFORMAT_UBYTE4N;
    pdesc.layout.attrs[2].buffer_index = 0;
    pdesc.layout.attrs[3].format = SG_VERTEXFORMAT_UBYTE4N;
    pdesc.layout.attrs[3].buffer_index = 0;
    pdesc.index_type = SG_INDEXTYPE_UINT16;
    pdesc.depth.compare = SG_COMPAREFUNC_LESS_EQUAL;
    pdesc.depth.write_enabled = true;
    pdesc.colors[0].blend.enabled = true;
    pdesc.colors[0].blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
    pdesc.colors[0].blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;

    m_pip = sg_make_pipeline(pdesc);

    m_vertices = new Vertex[vertex_capacity];
    m_vertex_count = 0;
    m_vertex_capacity = vertex_capacity;
}

BatchRenderer::~BatchRenderer() {
    sg_destroy_pipeline(m_pip);
    sg_destroy_buffer(m_vbo);
    sg_destroy_buffer(m_ibo);
    delete m_vertices;
}

BatchRenderer::BatchRenderer(BatchRenderer &&other) noexcept {
    m_pip = other.m_pip;
    m_vbo = other.m_vbo;
    m_ibo = other.m_ibo;
    m_vertices = other.m_vertices;
    m_vertex_count = other.m_vertex_count;
    m_vertex_capacity = other.m_vertex_capacity;
    u_mvp = other.u_mvp;
    u_texture = other.u_texture;

    other.m_pip.id = 0;
    other.m_vbo.id = 0;
    other.m_ibo.id = 0;
    other.m_vertices = nullptr;
}

BatchRenderer &BatchRenderer::operator=(BatchRenderer &&other) noexcept {
    if (this != &other) {
        sg_destroy_pipeline(m_pip);
        sg_destroy_buffer(m_vbo);
        sg_destroy_buffer(m_ibo);
        delete m_vertices;

        m_pip = other.m_pip;
        m_vbo = other.m_vbo;
        m_ibo = other.m_ibo;
        m_vertices = other.m_vertices;
        m_vertex_count = other.m_vertex_count;
        m_vertex_capacity = other.m_vertex_capacity;
        u_mvp = other.u_mvp;
        u_texture = other.u_texture;

        other.m_pip.id = 0;
        other.m_vbo.id = 0;
        other.m_ibo.id = 0;
        other.m_vertices = nullptr;
    }

    return *this;
}

void BatchRenderer::flush() {
    if (m_vertex_count == 0) {
        return;
    }

    int off = sg_append_buffer(
        m_vbo, {.ptr = m_vertices, .size = sizeof(Vertex) * m_vertex_count});

    sg_apply_pipeline(m_pip);
    sg_apply_bindings({
        .vertex_buffers = {m_vbo},
        .vertex_buffer_offsets = {off},
        .index_buffer = m_ibo,
        .fs_images = {u_texture},
    });
    sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, SG_RANGE(u_mvp));
    sg_draw(0, m_vertex_count * 6 / 4, 1);

    m_vertex_count = 0;
}

void BatchRenderer::push(Vertex vertex) {
    if (m_vertex_count == m_vertex_capacity) {
        flush();
    }

    m_vertices[m_vertex_count++] = vertex;
}

void BatchRenderer::mvp(Matrix mat) {
    flush();
    u_mvp = mat;
}

void BatchRenderer::texture(sg_image tex) {
    if (u_texture.id != tex.id) {
        flush();
        u_texture = tex;
    }
}
