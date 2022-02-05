#pragma once
/*
    #version:1# (machine generated, don't edit!)

    Generated by sokol-shdc (https://github.com/floooh/sokol-tools)

    Cmdline: sokol-shdc -i src\shd_default.glsl -o src\shd_default.h -l glsl330:glsl100:hlsl4 -b -f sokol_impl

    Overview:

        Shader program 'shd_default':
            Get shader desc: shd_default_shader_desc(sg_query_backend());
            Vertex shader: vert
                Attribute slots:
                    ATTR_vert_a_position = 0
                    ATTR_vert_a_texindex = 1
                    ATTR_vert_a_color = 2
                    ATTR_vert_a_fog = 3
                Uniform block 'vs_params':
                    C struct: vs_params_t
                    Bind slot: SLOT_vs_params = 0
            Fragment shader: frag
                Image 'u_texture':
                    Type: SG_IMAGETYPE_2D
                    Component Type: SG_SAMPLERTYPE_FLOAT
                    Bind slot: SLOT_u_texture = 0


    Shader descriptor structs:

        sg_shader shd_default = sg_make_shader(shd_default_shader_desc(sg_query_backend()));

    Vertex attribute locations for vertex shader 'vert':

        sg_pipeline pip = sg_make_pipeline(&(sg_pipeline_desc){
            .layout = {
                .attrs = {
                    [ATTR_vert_a_position] = { ... },
                    [ATTR_vert_a_texindex] = { ... },
                    [ATTR_vert_a_color] = { ... },
                    [ATTR_vert_a_fog] = { ... },
                },
            },
            ...});

    Image bind slots, use as index in sg_bindings.vs_images[] or .fs_images[]

        SLOT_u_texture = 0;

    Bind slot and C-struct for uniform block 'vs_params':

        vs_params_t vs_params = {
            .u_mvp = ...;
        };
        sg_apply_uniforms(SG_SHADERSTAGE_[VS|FS], SLOT_vs_params, &SG_RANGE(vs_params));

*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#if !defined(SOKOL_GFX_INCLUDED)
  #error "Please include sokol_gfx.h before shd_default.h"
#endif
#if !defined(SOKOL_SHDC_ALIGN)
  #if defined(_MSC_VER)
    #define SOKOL_SHDC_ALIGN(a) __declspec(align(a))
  #else
    #define SOKOL_SHDC_ALIGN(a) __attribute__((aligned(a)))
  #endif
#endif
const sg_shader_desc* shd_default_shader_desc(sg_backend backend);
#define ATTR_vert_a_position (0)
#define ATTR_vert_a_texindex (1)
#define ATTR_vert_a_color (2)
#define ATTR_vert_a_fog (3)
#define SLOT_u_texture (0)
#define SLOT_vs_params (0)
#pragma pack(push,1)
SOKOL_SHDC_ALIGN(16) typedef struct vs_params_t {
    RenMatrix u_mvp;
} vs_params_t;
#pragma pack(pop)
#if defined(SOKOL_SHDC_IMPL)
/*
    #version 330
    
    uniform vec4 vs_params[4];
    out vec2 v_texindex;
    layout(location = 1) in vec2 a_texindex;
    out vec4 v_color;
    layout(location = 2) in vec4 a_color;
    out vec4 v_fog;
    layout(location = 3) in vec4 a_fog;
    layout(location = 0) in vec3 a_position;
    
    void main()
    {
        v_texindex = a_texindex;
        v_color = a_color;
        v_fog = a_fog;
        gl_Position = mat4(vs_params[0], vs_params[1], vs_params[2], vs_params[3]) * vec4(a_position, 1.0);
    }
    
*/
static const char vert_source_glsl330[446] = {
    0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x33,0x33,0x30,0x0a,0x0a,0x75,0x6e,
    0x69,0x66,0x6f,0x72,0x6d,0x20,0x76,0x65,0x63,0x34,0x20,0x76,0x73,0x5f,0x70,0x61,
    0x72,0x61,0x6d,0x73,0x5b,0x34,0x5d,0x3b,0x0a,0x6f,0x75,0x74,0x20,0x76,0x65,0x63,
    0x32,0x20,0x76,0x5f,0x74,0x65,0x78,0x69,0x6e,0x64,0x65,0x78,0x3b,0x0a,0x6c,0x61,
    0x79,0x6f,0x75,0x74,0x28,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,
    0x31,0x29,0x20,0x69,0x6e,0x20,0x76,0x65,0x63,0x32,0x20,0x61,0x5f,0x74,0x65,0x78,
    0x69,0x6e,0x64,0x65,0x78,0x3b,0x0a,0x6f,0x75,0x74,0x20,0x76,0x65,0x63,0x34,0x20,
    0x76,0x5f,0x63,0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x6c,0x61,0x79,0x6f,0x75,0x74,0x28,
    0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x32,0x29,0x20,0x69,0x6e,
    0x20,0x76,0x65,0x63,0x34,0x20,0x61,0x5f,0x63,0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x6f,
    0x75,0x74,0x20,0x76,0x65,0x63,0x34,0x20,0x76,0x5f,0x66,0x6f,0x67,0x3b,0x0a,0x6c,
    0x61,0x79,0x6f,0x75,0x74,0x28,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,
    0x20,0x33,0x29,0x20,0x69,0x6e,0x20,0x76,0x65,0x63,0x34,0x20,0x61,0x5f,0x66,0x6f,
    0x67,0x3b,0x0a,0x6c,0x61,0x79,0x6f,0x75,0x74,0x28,0x6c,0x6f,0x63,0x61,0x74,0x69,
    0x6f,0x6e,0x20,0x3d,0x20,0x30,0x29,0x20,0x69,0x6e,0x20,0x76,0x65,0x63,0x33,0x20,
    0x61,0x5f,0x70,0x6f,0x73,0x69,0x74,0x69,0x6f,0x6e,0x3b,0x0a,0x0a,0x76,0x6f,0x69,
    0x64,0x20,0x6d,0x61,0x69,0x6e,0x28,0x29,0x0a,0x7b,0x0a,0x20,0x20,0x20,0x20,0x76,
    0x5f,0x74,0x65,0x78,0x69,0x6e,0x64,0x65,0x78,0x20,0x3d,0x20,0x61,0x5f,0x74,0x65,
    0x78,0x69,0x6e,0x64,0x65,0x78,0x3b,0x0a,0x20,0x20,0x20,0x20,0x76,0x5f,0x63,0x6f,
    0x6c,0x6f,0x72,0x20,0x3d,0x20,0x61,0x5f,0x63,0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x20,
    0x20,0x20,0x20,0x76,0x5f,0x66,0x6f,0x67,0x20,0x3d,0x20,0x61,0x5f,0x66,0x6f,0x67,
    0x3b,0x0a,0x20,0x20,0x20,0x20,0x67,0x6c,0x5f,0x50,0x6f,0x73,0x69,0x74,0x69,0x6f,
    0x6e,0x20,0x3d,0x20,0x6d,0x61,0x74,0x34,0x28,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,
    0x6d,0x73,0x5b,0x30,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,
    0x5b,0x31,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x32,
    0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x33,0x5d,0x29,
    0x20,0x2a,0x20,0x76,0x65,0x63,0x34,0x28,0x61,0x5f,0x70,0x6f,0x73,0x69,0x74,0x69,
    0x6f,0x6e,0x2c,0x20,0x31,0x2e,0x30,0x29,0x3b,0x0a,0x7d,0x0a,0x0a,0x00,
};
/*
    #version 330
    
    uniform sampler2D u_texture;
    
    in vec2 v_texindex;
    in vec4 v_color;
    layout(location = 0) out vec4 f_color;
    in vec4 v_fog;
    
    void main()
    {
        vec4 _21 = texture(u_texture, v_texindex);
        vec4 _25 = _21 * v_color;
        if (_25.w < 0.00999999977648258209228515625)
        {
            discard;
        }
        f_color = _25 + v_fog;
    }
    
*/
static const char frag_source_glsl330[336] = {
    0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x33,0x33,0x30,0x0a,0x0a,0x75,0x6e,
    0x69,0x66,0x6f,0x72,0x6d,0x20,0x73,0x61,0x6d,0x70,0x6c,0x65,0x72,0x32,0x44,0x20,
    0x75,0x5f,0x74,0x65,0x78,0x74,0x75,0x72,0x65,0x3b,0x0a,0x0a,0x69,0x6e,0x20,0x76,
    0x65,0x63,0x32,0x20,0x76,0x5f,0x74,0x65,0x78,0x69,0x6e,0x64,0x65,0x78,0x3b,0x0a,
    0x69,0x6e,0x20,0x76,0x65,0x63,0x34,0x20,0x76,0x5f,0x63,0x6f,0x6c,0x6f,0x72,0x3b,
    0x0a,0x6c,0x61,0x79,0x6f,0x75,0x74,0x28,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,
    0x20,0x3d,0x20,0x30,0x29,0x20,0x6f,0x75,0x74,0x20,0x76,0x65,0x63,0x34,0x20,0x66,
    0x5f,0x63,0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x69,0x6e,0x20,0x76,0x65,0x63,0x34,0x20,
    0x76,0x5f,0x66,0x6f,0x67,0x3b,0x0a,0x0a,0x76,0x6f,0x69,0x64,0x20,0x6d,0x61,0x69,
    0x6e,0x28,0x29,0x0a,0x7b,0x0a,0x20,0x20,0x20,0x20,0x76,0x65,0x63,0x34,0x20,0x5f,
    0x32,0x31,0x20,0x3d,0x20,0x74,0x65,0x78,0x74,0x75,0x72,0x65,0x28,0x75,0x5f,0x74,
    0x65,0x78,0x74,0x75,0x72,0x65,0x2c,0x20,0x76,0x5f,0x74,0x65,0x78,0x69,0x6e,0x64,
    0x65,0x78,0x29,0x3b,0x0a,0x20,0x20,0x20,0x20,0x76,0x65,0x63,0x34,0x20,0x5f,0x32,
    0x35,0x20,0x3d,0x20,0x5f,0x32,0x31,0x20,0x2a,0x20,0x76,0x5f,0x63,0x6f,0x6c,0x6f,
    0x72,0x3b,0x0a,0x20,0x20,0x20,0x20,0x69,0x66,0x20,0x28,0x5f,0x32,0x35,0x2e,0x77,
    0x20,0x3c,0x20,0x30,0x2e,0x30,0x30,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x37,0x37,
    0x36,0x34,0x38,0x32,0x35,0x38,0x32,0x30,0x39,0x32,0x32,0x38,0x35,0x31,0x35,0x36,
    0x32,0x35,0x29,0x0a,0x20,0x20,0x20,0x20,0x7b,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x64,0x69,0x73,0x63,0x61,0x72,0x64,0x3b,0x0a,0x20,0x20,0x20,0x20,0x7d,
    0x0a,0x20,0x20,0x20,0x20,0x66,0x5f,0x63,0x6f,0x6c,0x6f,0x72,0x20,0x3d,0x20,0x5f,
    0x32,0x35,0x20,0x2b,0x20,0x76,0x5f,0x66,0x6f,0x67,0x3b,0x0a,0x7d,0x0a,0x0a,0x00,

};
/*
    #version 100
    
    uniform vec4 vs_params[4];
    varying vec2 v_texindex;
    attribute vec2 a_texindex;
    varying vec4 v_color;
    attribute vec4 a_color;
    varying vec4 v_fog;
    attribute vec4 a_fog;
    attribute vec3 a_position;
    
    void main()
    {
        v_texindex = a_texindex;
        v_color = a_color;
        v_fog = a_fog;
        gl_Position = mat4(vs_params[0], vs_params[1], vs_params[2], vs_params[3]) * vec4(a_position, 1.0);
    }
    
*/
static const char vert_source_glsl100[402] = {
    0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x31,0x30,0x30,0x0a,0x0a,0x75,0x6e,
    0x69,0x66,0x6f,0x72,0x6d,0x20,0x76,0x65,0x63,0x34,0x20,0x76,0x73,0x5f,0x70,0x61,
    0x72,0x61,0x6d,0x73,0x5b,0x34,0x5d,0x3b,0x0a,0x76,0x61,0x72,0x79,0x69,0x6e,0x67,
    0x20,0x76,0x65,0x63,0x32,0x20,0x76,0x5f,0x74,0x65,0x78,0x69,0x6e,0x64,0x65,0x78,
    0x3b,0x0a,0x61,0x74,0x74,0x72,0x69,0x62,0x75,0x74,0x65,0x20,0x76,0x65,0x63,0x32,
    0x20,0x61,0x5f,0x74,0x65,0x78,0x69,0x6e,0x64,0x65,0x78,0x3b,0x0a,0x76,0x61,0x72,
    0x79,0x69,0x6e,0x67,0x20,0x76,0x65,0x63,0x34,0x20,0x76,0x5f,0x63,0x6f,0x6c,0x6f,
    0x72,0x3b,0x0a,0x61,0x74,0x74,0x72,0x69,0x62,0x75,0x74,0x65,0x20,0x76,0x65,0x63,
    0x34,0x20,0x61,0x5f,0x63,0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x76,0x61,0x72,0x79,0x69,
    0x6e,0x67,0x20,0x76,0x65,0x63,0x34,0x20,0x76,0x5f,0x66,0x6f,0x67,0x3b,0x0a,0x61,
    0x74,0x74,0x72,0x69,0x62,0x75,0x74,0x65,0x20,0x76,0x65,0x63,0x34,0x20,0x61,0x5f,
    0x66,0x6f,0x67,0x3b,0x0a,0x61,0x74,0x74,0x72,0x69,0x62,0x75,0x74,0x65,0x20,0x76,
    0x65,0x63,0x33,0x20,0x61,0x5f,0x70,0x6f,0x73,0x69,0x74,0x69,0x6f,0x6e,0x3b,0x0a,
    0x0a,0x76,0x6f,0x69,0x64,0x20,0x6d,0x61,0x69,0x6e,0x28,0x29,0x0a,0x7b,0x0a,0x20,
    0x20,0x20,0x20,0x76,0x5f,0x74,0x65,0x78,0x69,0x6e,0x64,0x65,0x78,0x20,0x3d,0x20,
    0x61,0x5f,0x74,0x65,0x78,0x69,0x6e,0x64,0x65,0x78,0x3b,0x0a,0x20,0x20,0x20,0x20,
    0x76,0x5f,0x63,0x6f,0x6c,0x6f,0x72,0x20,0x3d,0x20,0x61,0x5f,0x63,0x6f,0x6c,0x6f,
    0x72,0x3b,0x0a,0x20,0x20,0x20,0x20,0x76,0x5f,0x66,0x6f,0x67,0x20,0x3d,0x20,0x61,
    0x5f,0x66,0x6f,0x67,0x3b,0x0a,0x20,0x20,0x20,0x20,0x67,0x6c,0x5f,0x50,0x6f,0x73,
    0x69,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x6d,0x61,0x74,0x34,0x28,0x76,0x73,0x5f,
    0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x30,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,
    0x72,0x61,0x6d,0x73,0x5b,0x31,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,
    0x6d,0x73,0x5b,0x32,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,
    0x5b,0x33,0x5d,0x29,0x20,0x2a,0x20,0x76,0x65,0x63,0x34,0x28,0x61,0x5f,0x70,0x6f,
    0x73,0x69,0x74,0x69,0x6f,0x6e,0x2c,0x20,0x31,0x2e,0x30,0x29,0x3b,0x0a,0x7d,0x0a,
    0x0a,0x00,
};
/*
    #version 100
    precision mediump float;
    precision highp int;
    
    uniform highp sampler2D u_texture;
    
    varying highp vec2 v_texindex;
    varying highp vec4 v_color;
    varying highp vec4 v_fog;
    
    void main()
    {
        highp vec4 _21 = texture2D(u_texture, v_texindex);
        highp vec4 _25 = _21 * v_color;
        if (_25.w < 0.00999999977648258209228515625)
        {
            discard;
        }
        gl_FragData[0] = _25 + v_fog;
    }
    
*/
static const char frag_source_glsl100[403] = {
    0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x31,0x30,0x30,0x0a,0x70,0x72,0x65,
    0x63,0x69,0x73,0x69,0x6f,0x6e,0x20,0x6d,0x65,0x64,0x69,0x75,0x6d,0x70,0x20,0x66,
    0x6c,0x6f,0x61,0x74,0x3b,0x0a,0x70,0x72,0x65,0x63,0x69,0x73,0x69,0x6f,0x6e,0x20,
    0x68,0x69,0x67,0x68,0x70,0x20,0x69,0x6e,0x74,0x3b,0x0a,0x0a,0x75,0x6e,0x69,0x66,
    0x6f,0x72,0x6d,0x20,0x68,0x69,0x67,0x68,0x70,0x20,0x73,0x61,0x6d,0x70,0x6c,0x65,
    0x72,0x32,0x44,0x20,0x75,0x5f,0x74,0x65,0x78,0x74,0x75,0x72,0x65,0x3b,0x0a,0x0a,
    0x76,0x61,0x72,0x79,0x69,0x6e,0x67,0x20,0x68,0x69,0x67,0x68,0x70,0x20,0x76,0x65,
    0x63,0x32,0x20,0x76,0x5f,0x74,0x65,0x78,0x69,0x6e,0x64,0x65,0x78,0x3b,0x0a,0x76,
    0x61,0x72,0x79,0x69,0x6e,0x67,0x20,0x68,0x69,0x67,0x68,0x70,0x20,0x76,0x65,0x63,
    0x34,0x20,0x76,0x5f,0x63,0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x76,0x61,0x72,0x79,0x69,
    0x6e,0x67,0x20,0x68,0x69,0x67,0x68,0x70,0x20,0x76,0x65,0x63,0x34,0x20,0x76,0x5f,
    0x66,0x6f,0x67,0x3b,0x0a,0x0a,0x76,0x6f,0x69,0x64,0x20,0x6d,0x61,0x69,0x6e,0x28,
    0x29,0x0a,0x7b,0x0a,0x20,0x20,0x20,0x20,0x68,0x69,0x67,0x68,0x70,0x20,0x76,0x65,
    0x63,0x34,0x20,0x5f,0x32,0x31,0x20,0x3d,0x20,0x74,0x65,0x78,0x74,0x75,0x72,0x65,
    0x32,0x44,0x28,0x75,0x5f,0x74,0x65,0x78,0x74,0x75,0x72,0x65,0x2c,0x20,0x76,0x5f,
    0x74,0x65,0x78,0x69,0x6e,0x64,0x65,0x78,0x29,0x3b,0x0a,0x20,0x20,0x20,0x20,0x68,
    0x69,0x67,0x68,0x70,0x20,0x76,0x65,0x63,0x34,0x20,0x5f,0x32,0x35,0x20,0x3d,0x20,
    0x5f,0x32,0x31,0x20,0x2a,0x20,0x76,0x5f,0x63,0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x20,
    0x20,0x20,0x20,0x69,0x66,0x20,0x28,0x5f,0x32,0x35,0x2e,0x77,0x20,0x3c,0x20,0x30,
    0x2e,0x30,0x30,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x37,0x37,0x36,0x34,0x38,0x32,
    0x35,0x38,0x32,0x30,0x39,0x32,0x32,0x38,0x35,0x31,0x35,0x36,0x32,0x35,0x29,0x0a,
    0x20,0x20,0x20,0x20,0x7b,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x64,0x69,
    0x73,0x63,0x61,0x72,0x64,0x3b,0x0a,0x20,0x20,0x20,0x20,0x7d,0x0a,0x20,0x20,0x20,
    0x20,0x67,0x6c,0x5f,0x46,0x72,0x61,0x67,0x44,0x61,0x74,0x61,0x5b,0x30,0x5d,0x20,
    0x3d,0x20,0x5f,0x32,0x35,0x20,0x2b,0x20,0x76,0x5f,0x66,0x6f,0x67,0x3b,0x0a,0x7d,
    0x0a,0x0a,0x00,
};
/*
    cbuffer vs_params : register(b0)
    {
        row_major float4x4 _35_u_mvp : packoffset(c0);
    };
    
    
    static float4 gl_Position;
    static float2 v_texindex;
    static float2 a_texindex;
    static float4 v_color;
    static float4 a_color;
    static float4 v_fog;
    static float4 a_fog;
    static float3 a_position;
    
    struct SPIRV_Cross_Input
    {
        float3 a_position : TEXCOORD0;
        float2 a_texindex : TEXCOORD1;
        float4 a_color : TEXCOORD2;
        float4 a_fog : TEXCOORD3;
    };
    
    struct SPIRV_Cross_Output
    {
        float2 v_texindex : TEXCOORD0;
        float4 v_color : TEXCOORD1;
        float4 v_fog : TEXCOORD2;
        float4 gl_Position : SV_Position;
    };
    
    #line 20 "src\shd_default.glsl"
    void vert_main()
    {
    #line 20 "src\shd_default.glsl"
        v_texindex = a_texindex;
    #line 21 "src\shd_default.glsl"
        v_color = a_color;
    #line 22 "src\shd_default.glsl"
        v_fog = a_fog;
    #line 23 "src\shd_default.glsl"
        gl_Position = mul(float4(a_position, 1.0f), _35_u_mvp);
    }
    
    SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
    {
        a_texindex = stage_input.a_texindex;
        a_color = stage_input.a_color;
        a_fog = stage_input.a_fog;
        a_position = stage_input.a_position;
        vert_main();
        SPIRV_Cross_Output stage_output;
        stage_output.gl_Position = gl_Position;
        stage_output.v_texindex = v_texindex;
        stage_output.v_color = v_color;
        stage_output.v_fog = v_fog;
        return stage_output;
    }
*/
static const uint8_t vert_bytecode_hlsl4[980] = {
    0x44,0x58,0x42,0x43,0xdc,0xab,0xdb,0x37,0x2a,0x94,0x40,0x9f,0xad,0x3e,0x8e,0xc0,
    0x73,0x7f,0xcf,0x98,0x01,0x00,0x00,0x00,0xd4,0x03,0x00,0x00,0x05,0x00,0x00,0x00,
    0x34,0x00,0x00,0x00,0xf8,0x00,0x00,0x00,0x74,0x01,0x00,0x00,0xfc,0x01,0x00,0x00,
    0x58,0x03,0x00,0x00,0x52,0x44,0x45,0x46,0xbc,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
    0x48,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x1c,0x00,0x00,0x00,0x00,0x04,0xfe,0xff,
    0x10,0x81,0x00,0x00,0x94,0x00,0x00,0x00,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,
    0x73,0x00,0xab,0xab,0x3c,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x60,0x00,0x00,0x00,
    0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x84,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x5f,0x33,0x35,0x5f,0x75,0x5f,0x6d,0x76,0x70,0x00,0xab,0xab,
    0x02,0x00,0x03,0x00,0x04,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x4d,0x69,0x63,0x72,0x6f,0x73,0x6f,0x66,0x74,0x20,0x28,0x52,0x29,0x20,0x48,0x4c,
    0x53,0x4c,0x20,0x53,0x68,0x61,0x64,0x65,0x72,0x20,0x43,0x6f,0x6d,0x70,0x69,0x6c,
    0x65,0x72,0x20,0x31,0x30,0x2e,0x31,0x00,0x49,0x53,0x47,0x4e,0x74,0x00,0x00,0x00,
    0x04,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x07,0x00,0x00,
    0x68,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,
    0x01,0x00,0x00,0x00,0x03,0x03,0x00,0x00,0x68,0x00,0x00,0x00,0x02,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x0f,0x0f,0x00,0x00,
    0x68,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,
    0x03,0x00,0x00,0x00,0x0f,0x0f,0x00,0x00,0x54,0x45,0x58,0x43,0x4f,0x4f,0x52,0x44,
    0x00,0xab,0xab,0xab,0x4f,0x53,0x47,0x4e,0x80,0x00,0x00,0x00,0x04,0x00,0x00,0x00,
    0x08,0x00,0x00,0x00,0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x0c,0x00,0x00,0x68,0x00,0x00,0x00,
    0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
    0x0f,0x00,0x00,0x00,0x68,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x0f,0x00,0x00,0x00,0x71,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,
    0x0f,0x00,0x00,0x00,0x54,0x45,0x58,0x43,0x4f,0x4f,0x52,0x44,0x00,0x53,0x56,0x5f,
    0x50,0x6f,0x73,0x69,0x74,0x69,0x6f,0x6e,0x00,0xab,0xab,0xab,0x53,0x48,0x44,0x52,
    0x54,0x01,0x00,0x00,0x40,0x00,0x01,0x00,0x55,0x00,0x00,0x00,0x59,0x00,0x00,0x04,
    0x46,0x8e,0x20,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x5f,0x00,0x00,0x03,
    0x72,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x5f,0x00,0x00,0x03,0x32,0x10,0x10,0x00,
    0x01,0x00,0x00,0x00,0x5f,0x00,0x00,0x03,0xf2,0x10,0x10,0x00,0x02,0x00,0x00,0x00,
    0x5f,0x00,0x00,0x03,0xf2,0x10,0x10,0x00,0x03,0x00,0x00,0x00,0x65,0x00,0x00,0x03,
    0x32,0x20,0x10,0x00,0x00,0x00,0x00,0x00,0x65,0x00,0x00,0x03,0xf2,0x20,0x10,0x00,
    0x01,0x00,0x00,0x00,0x65,0x00,0x00,0x03,0xf2,0x20,0x10,0x00,0x02,0x00,0x00,0x00,
    0x67,0x00,0x00,0x04,0xf2,0x20,0x10,0x00,0x03,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
    0x68,0x00,0x00,0x02,0x01,0x00,0x00,0x00,0x36,0x00,0x00,0x05,0x32,0x20,0x10,0x00,
    0x00,0x00,0x00,0x00,0x46,0x10,0x10,0x00,0x01,0x00,0x00,0x00,0x36,0x00,0x00,0x05,
    0xf2,0x20,0x10,0x00,0x01,0x00,0x00,0x00,0x46,0x1e,0x10,0x00,0x02,0x00,0x00,0x00,
    0x36,0x00,0x00,0x05,0xf2,0x20,0x10,0x00,0x02,0x00,0x00,0x00,0x46,0x1e,0x10,0x00,
    0x03,0x00,0x00,0x00,0x38,0x00,0x00,0x08,0xf2,0x00,0x10,0x00,0x00,0x00,0x00,0x00,
    0x56,0x15,0x10,0x00,0x00,0x00,0x00,0x00,0x46,0x8e,0x20,0x00,0x00,0x00,0x00,0x00,
    0x01,0x00,0x00,0x00,0x32,0x00,0x00,0x0a,0xf2,0x00,0x10,0x00,0x00,0x00,0x00,0x00,
    0x06,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x46,0x8e,0x20,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x46,0x0e,0x10,0x00,0x00,0x00,0x00,0x00,0x32,0x00,0x00,0x0a,
    0xf2,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0xa6,0x1a,0x10,0x00,0x00,0x00,0x00,0x00,
    0x46,0x8e,0x20,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x46,0x0e,0x10,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0xf2,0x20,0x10,0x00,0x03,0x00,0x00,0x00,
    0x46,0x0e,0x10,0x00,0x00,0x00,0x00,0x00,0x46,0x8e,0x20,0x00,0x00,0x00,0x00,0x00,
    0x03,0x00,0x00,0x00,0x3e,0x00,0x00,0x01,0x53,0x54,0x41,0x54,0x74,0x00,0x00,0x00,
    0x08,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,
    0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,
};
/*
    Texture2D<float4> u_texture : register(t0);
    SamplerState _u_texture_sampler : register(s0);
    
    static float2 v_texindex;
    static float4 v_color;
    static float4 f_color;
    static float4 v_fog;
    
    struct SPIRV_Cross_Input
    {
        float2 v_texindex : TEXCOORD0;
        float4 v_color : TEXCOORD1;
        float4 v_fog : TEXCOORD2;
    };
    
    struct SPIRV_Cross_Output
    {
        float4 f_color : SV_Target0;
    };
    
    #line 15 "src\shd_default.glsl"
    void frag_main()
    {
    #line 15 "src\shd_default.glsl"
        float4 _21 = u_texture.Sample(_u_texture_sampler, v_texindex);
        float4 _25 = _21 * v_color;
    #line 16 "src\shd_default.glsl"
        if (_25.w < 0.00999999977648258209228515625f)
        {
    #line 17 "src\shd_default.glsl"
            discard;
        }
    #line 20 "src\shd_default.glsl"
        f_color = _25 + v_fog;
    }
    
    SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
    {
        v_texindex = stage_input.v_texindex;
        v_color = stage_input.v_color;
        v_fog = stage_input.v_fog;
        frag_main();
        SPIRV_Cross_Output stage_output;
        stage_output.f_color = f_color;
        return stage_output;
    }
*/
static const uint8_t frag_bytecode_hlsl4[744] = {
    0x44,0x58,0x42,0x43,0x8c,0xb2,0x2a,0x32,0x9e,0x95,0xa3,0xc0,0x72,0xc8,0x19,0xa4,
    0x98,0x3d,0x06,0x45,0x01,0x00,0x00,0x00,0xe8,0x02,0x00,0x00,0x05,0x00,0x00,0x00,
    0x34,0x00,0x00,0x00,0xe0,0x00,0x00,0x00,0x44,0x01,0x00,0x00,0x78,0x01,0x00,0x00,
    0x6c,0x02,0x00,0x00,0x52,0x44,0x45,0x46,0xa4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x1c,0x00,0x00,0x00,0x00,0x04,0xff,0xff,
    0x10,0x81,0x00,0x00,0x79,0x00,0x00,0x00,0x5c,0x00,0x00,0x00,0x03,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x6f,0x00,0x00,0x00,0x02,0x00,0x00,0x00,
    0x05,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
    0x01,0x00,0x00,0x00,0x0d,0x00,0x00,0x00,0x5f,0x75,0x5f,0x74,0x65,0x78,0x74,0x75,
    0x72,0x65,0x5f,0x73,0x61,0x6d,0x70,0x6c,0x65,0x72,0x00,0x75,0x5f,0x74,0x65,0x78,
    0x74,0x75,0x72,0x65,0x00,0x4d,0x69,0x63,0x72,0x6f,0x73,0x6f,0x66,0x74,0x20,0x28,
    0x52,0x29,0x20,0x48,0x4c,0x53,0x4c,0x20,0x53,0x68,0x61,0x64,0x65,0x72,0x20,0x43,
    0x6f,0x6d,0x70,0x69,0x6c,0x65,0x72,0x20,0x31,0x30,0x2e,0x31,0x00,0xab,0xab,0xab,
    0x49,0x53,0x47,0x4e,0x5c,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x08,0x00,0x00,0x00,
    0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x03,0x03,0x00,0x00,0x50,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x0f,0x0f,0x00,0x00,
    0x50,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,
    0x02,0x00,0x00,0x00,0x0f,0x0f,0x00,0x00,0x54,0x45,0x58,0x43,0x4f,0x4f,0x52,0x44,
    0x00,0xab,0xab,0xab,0x4f,0x53,0x47,0x4e,0x2c,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
    0x08,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x00,0x00,0x00,0x53,0x56,0x5f,0x54,
    0x61,0x72,0x67,0x65,0x74,0x00,0xab,0xab,0x53,0x48,0x44,0x52,0xec,0x00,0x00,0x00,
    0x40,0x00,0x00,0x00,0x3b,0x00,0x00,0x00,0x5a,0x00,0x00,0x03,0x00,0x60,0x10,0x00,
    0x00,0x00,0x00,0x00,0x58,0x18,0x00,0x04,0x00,0x70,0x10,0x00,0x00,0x00,0x00,0x00,
    0x55,0x55,0x00,0x00,0x62,0x10,0x00,0x03,0x32,0x10,0x10,0x00,0x00,0x00,0x00,0x00,
    0x62,0x10,0x00,0x03,0xf2,0x10,0x10,0x00,0x01,0x00,0x00,0x00,0x62,0x10,0x00,0x03,
    0xf2,0x10,0x10,0x00,0x02,0x00,0x00,0x00,0x65,0x00,0x00,0x03,0xf2,0x20,0x10,0x00,
    0x00,0x00,0x00,0x00,0x68,0x00,0x00,0x02,0x02,0x00,0x00,0x00,0x45,0x00,0x00,0x09,
    0xf2,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x46,0x10,0x10,0x00,0x00,0x00,0x00,0x00,
    0x46,0x7e,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x10,0x00,0x00,0x00,0x00,0x00,
    0x38,0x00,0x00,0x07,0x12,0x00,0x10,0x00,0x01,0x00,0x00,0x00,0x3a,0x00,0x10,0x00,
    0x00,0x00,0x00,0x00,0x3a,0x10,0x10,0x00,0x01,0x00,0x00,0x00,0x31,0x00,0x00,0x07,
    0x12,0x00,0x10,0x00,0x01,0x00,0x00,0x00,0x0a,0x00,0x10,0x00,0x01,0x00,0x00,0x00,
    0x01,0x40,0x00,0x00,0x0a,0xd7,0x23,0x3c,0x0d,0x00,0x04,0x03,0x0a,0x00,0x10,0x00,
    0x01,0x00,0x00,0x00,0x32,0x00,0x00,0x09,0xf2,0x20,0x10,0x00,0x00,0x00,0x00,0x00,
    0x46,0x0e,0x10,0x00,0x00,0x00,0x00,0x00,0x46,0x1e,0x10,0x00,0x01,0x00,0x00,0x00,
    0x46,0x1e,0x10,0x00,0x02,0x00,0x00,0x00,0x3e,0x00,0x00,0x01,0x53,0x54,0x41,0x54,
    0x74,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x04,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
const sg_shader_desc* shd_default_shader_desc(sg_backend backend) {
  if (backend == SG_BACKEND_GLCORE33) {
    static sg_shader_desc desc;
    static bool valid;
    if (!valid) {
      valid = true;
      desc.attrs[0].name = "a_position";
      desc.attrs[1].name = "a_texindex";
      desc.attrs[2].name = "a_color";
      desc.attrs[3].name = "a_fog";
      desc.vs.source = vert_source_glsl330;
      desc.vs.entry = "main";
      desc.vs.uniform_blocks[0].size = 64;
      desc.vs.uniform_blocks[0].layout = SG_UNIFORMLAYOUT_STD140;
      desc.vs.uniform_blocks[0].uniforms[0].name = "vs_params";
      desc.vs.uniform_blocks[0].uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;
      desc.vs.uniform_blocks[0].uniforms[0].array_count = 4;
      desc.fs.source = frag_source_glsl330;
      desc.fs.entry = "main";
      desc.fs.images[0].name = "u_texture";
      desc.fs.images[0].image_type = SG_IMAGETYPE_2D;
      desc.fs.images[0].sampler_type = SG_SAMPLERTYPE_FLOAT;
      desc.label = "shd_default_shader";
    }
    return &desc;
  }
  if (backend == SG_BACKEND_GLES2) {
    static sg_shader_desc desc;
    static bool valid;
    if (!valid) {
      valid = true;
      desc.attrs[0].name = "a_position";
      desc.attrs[1].name = "a_texindex";
      desc.attrs[2].name = "a_color";
      desc.attrs[3].name = "a_fog";
      desc.vs.source = vert_source_glsl100;
      desc.vs.entry = "main";
      desc.vs.uniform_blocks[0].size = 64;
      desc.vs.uniform_blocks[0].layout = SG_UNIFORMLAYOUT_STD140;
      desc.vs.uniform_blocks[0].uniforms[0].name = "vs_params";
      desc.vs.uniform_blocks[0].uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;
      desc.vs.uniform_blocks[0].uniforms[0].array_count = 4;
      desc.fs.source = frag_source_glsl100;
      desc.fs.entry = "main";
      desc.fs.images[0].name = "u_texture";
      desc.fs.images[0].image_type = SG_IMAGETYPE_2D;
      desc.fs.images[0].sampler_type = SG_SAMPLERTYPE_FLOAT;
      desc.label = "shd_default_shader";
    }
    return &desc;
  }
  if (backend == SG_BACKEND_D3D11) {
    static sg_shader_desc desc;
    static bool valid;
    if (!valid) {
      valid = true;
      desc.attrs[0].sem_name = "TEXCOORD";
      desc.attrs[0].sem_index = 0;
      desc.attrs[1].sem_name = "TEXCOORD";
      desc.attrs[1].sem_index = 1;
      desc.attrs[2].sem_name = "TEXCOORD";
      desc.attrs[2].sem_index = 2;
      desc.attrs[3].sem_name = "TEXCOORD";
      desc.attrs[3].sem_index = 3;
      desc.vs.bytecode.ptr = vert_bytecode_hlsl4;
      desc.vs.bytecode.size = 980;
      desc.vs.entry = "main";
      desc.vs.uniform_blocks[0].size = 64;
      desc.vs.uniform_blocks[0].layout = SG_UNIFORMLAYOUT_STD140;
      desc.fs.bytecode.ptr = frag_bytecode_hlsl4;
      desc.fs.bytecode.size = 744;
      desc.fs.entry = "main";
      desc.fs.images[0].name = "u_texture";
      desc.fs.images[0].image_type = SG_IMAGETYPE_2D;
      desc.fs.images[0].sampler_type = SG_SAMPLERTYPE_FLOAT;
      desc.label = "shd_default_shader";
    }
    return &desc;
  }
  return 0;
}
#endif /* SOKOL_SHDC_IMPL */
