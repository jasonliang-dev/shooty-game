@ctype mat4 RenMatrix

@vs vert
uniform vs_params {
    mat4 u_mvp;
};

in vec3 a_position;
in vec2 a_texindex;
in vec4 a_color;
in vec4 a_fog;

out vec2 v_texindex;
out vec4 v_color;
out vec4 v_fog;

void main() {
    v_texindex = a_texindex;
    v_color = a_color;
    v_fog = a_fog;
    gl_Position = u_mvp * vec4(a_position, 1);
}
@end

@fs frag
in vec2 v_texindex;
in vec4 v_color;
in vec4 v_fog;

out vec4 f_color;

uniform sampler2D u_texture;

void main() {
    vec4 result = texture(u_texture, v_texindex) * v_color;
    if (result.a < 0.1) {
        discard;
    }

    f_color = result + v_fog;
}
@end

@program shd_default vert frag
