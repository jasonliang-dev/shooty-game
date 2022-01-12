#pragma once

#include "deps/sokol_gfx.h"

struct Vertex {
    float pos[3];
    float uv[2];
    unsigned char color[4];
    unsigned char fog[4];
};

union Matrix {
    float cols[4][4];
    float arr[16];
};

struct BatchRenderer {
    BatchRenderer() = default;
    BatchRenderer(int vertex_capacity);

    ~BatchRenderer();

    BatchRenderer(const BatchRenderer &) = delete;
    BatchRenderer &operator=(const BatchRenderer &) = delete;

    BatchRenderer(BatchRenderer &&) noexcept;
    BatchRenderer &operator=(BatchRenderer &&) noexcept;

    void flush();
    void push(Vertex vertex);
    void push(Vertex *vertices, int n);
    void mvp(Matrix mat);
    void texture(sg_image tex);

  private:
    sg_pipeline m_pip{};

    sg_buffer m_vbo{};
    sg_buffer m_ibo{};

    Vertex *m_vertices = nullptr;
    int m_vertex_count = 0;
    int m_vertex_capacity = 0;

    Matrix u_mvp{};
    sg_image u_texture{};
};
