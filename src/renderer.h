#pragma once

#include "deps/sokol_gfx.h"
#include "types.h"

struct RenVertex {
    float pos[3];
    float uv[2];
    u8 color[4];
    u8 fog[4];
};

union RenMatrix {
    float cols[4][4];
    float arr[16];
};

struct Renderer {
    void create(int vertex_capacity);
    void destroy();

    void begin();
    void flush();
    void push(RenVertex vertex);
    void push(RenVertex *vertices, int n);
    void mvp(RenMatrix mat);
    void texture(sg_image tex);

    int draw_count() const;
    sg_pipeline pip() const;
    sg_buffer quad_ibo() const;
    RenMatrix mvp() const;
    sg_image texture() const;

  private:
    sg_pipeline m_pip{};

    sg_buffer m_vbo{};
    sg_buffer m_ibo{};

    RenVertex *m_vertices = nullptr;
    int m_vertex_count = 0;
    int m_vertex_capacity = 0;

    RenMatrix u_mvp{};
    sg_image u_texture{};

    int m_draw_count = 0;
};
