#pragma once

#include "deps/sokol_gfx.h"
#include "ints.h"

struct Vertex {
    float pos[3];
    float uv[2];
    u8 color[4];
    u8 fog[4];
};

union Matrix {
    float cols[4][4];
    float arr[16];
};

struct Renderer {
    Renderer() = default;
    Renderer(int vertex_capacity);

    ~Renderer();

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

    Renderer(Renderer &&) noexcept;
    Renderer &operator=(Renderer &&) noexcept;

    void begin();
    void flush();
    void push(Vertex vertex);
    void push(Vertex *vertices, int n);
    void mvp(Matrix mat);
    void texture(sg_image tex);
    int draw_count();

  private:
    sg_pipeline m_pip{};

    sg_buffer m_vbo{};
    sg_buffer m_ibo{};

    Vertex *m_vertices = nullptr;
    int m_vertex_count = 0;
    int m_vertex_capacity = 0;

    Matrix u_mvp{};
    sg_image u_texture{};

    int m_draw_count = 0;
};
