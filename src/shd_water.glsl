@ctype mat4 Matrix

@vs vert
uniform vs_uniforms_water {
    mat4 u_mvp;
};

in vec3 a_position;

out vec3 v_position;

void main() {
    v_position = a_position;
    gl_Position = u_mvp * vec4(a_position, 1);
}
@end

@fs frag
uniform fs_uniforms_water {
    float u_scale;
    float u_time;
};

in vec3 v_position;

out vec4 f_color;

vec2 random2(vec2 v) {
    return -1.0 + 2.0 * fract(sin(vec2(dot(v, vec2(127.1, 311.7)), dot(v, vec2(269.5, 183.3)))) * 43758.5453123);
}

float noise(vec2 v) {
    vec2 i = floor(v);
    vec2 f = fract(v);
    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(mix(dot(random2(i + vec2(0.0, 0.0)), f - vec2(0.0, 0.0)),
                   dot(random2(i + vec2(1.0, 0.0)), f - vec2(1.0, 0.0)), u.x),
               mix(dot(random2(i + vec2(0.0, 1.0)), f - vec2(0.0, 1.0)),
                   dot(random2(i + vec2(1.0, 1.0)), f - vec2(1.0, 1.0)), u.x), u.y);
}

void main() {
    vec2 uv = v_position.xy * u_scale;
    uv -= u_time * 0.5;

    vec2 duv = v_position.xy * u_scale;
    float dxy = u_time * 0.25;
    vec2 distort = vec2(noise(duv + dxy), noise(duv + dxy + 1000.0));
    distort = distort * 0.5 + 0.5;

    float dist = 0.928;

    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            vec2 neighbor = vec2(float(x), float(y));

            vec2 point = random2(floor(uv) + neighbor);
            point = 0.5 + 0.5 * sin(u_time * 0.8 + 6.0 * point + distort * 4.0);

            dist = min(dist, length(neighbor + point - fract(uv)));
        }
    }

    float value = pow(dist, 8.0);
    vec3 color = value > 0.5 ? vec3(1.0) : vec3(0.852,0.957,0.995);

    f_color = vec4(color, 1.0);
}
@end

@program shd_water vert frag
