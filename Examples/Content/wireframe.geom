#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3    o_position;
    vec3    o_normal;
    vec3    o_tangent;
    vec3    o_bitangent;
    vec2    o_texcoord0;
    vec2    o_texcoord1;
    vec2    o_texcoord2;
} gs_in[];

out vec3    o_position;
out vec3    o_normal;
out vec3    o_tangent;
out vec3    o_bitangent;
out vec2    o_texcoord0;
out vec2    o_texcoord1;
out vec2    o_texcoord2;

void main() {
    gl_Position = gl_in[0].gl_Position;
    o_position = gs_in[0].o_position;
    o_tangent = gs_in[0].o_tangent;
    o_bitangent = gs_in[0].o_bitangent;
    o_texcoord0 = gs_in[0].o_texcoord0;
    o_texcoord1 = gs_in[0].o_texcoord1;
    o_texcoord2 = gs_in[0].o_texcoord2;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    o_position = gs_in[1].o_position;
    o_normal = gs_in[1].o_normal;
    o_tangent = gs_in[1].o_tangent;
    o_bitangent = gs_in[1].o_bitangent;
    o_texcoord0 = gs_in[1].o_texcoord0;
    o_texcoord1 = gs_in[1].o_texcoord1;
    o_texcoord2 = gs_in[1].o_texcoord2;
    EmitVertex();

    EndPrimitive();

    gl_Position = gl_in[1].gl_Position;
    o_position = gs_in[1].o_position;
    o_normal = gs_in[1].o_normal;
    o_tangent = gs_in[1].o_tangent;
    o_bitangent = gs_in[1].o_bitangent;
    o_texcoord0 = gs_in[1].o_texcoord0;
    o_texcoord1 = gs_in[1].o_texcoord1;
    o_texcoord2 = gs_in[1].o_texcoord2;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    o_position = gs_in[2].o_position;
    o_normal = gs_in[2].o_normal;
    o_tangent = gs_in[2].o_tangent;
    o_bitangent = gs_in[2].o_bitangent;
    o_texcoord0 = gs_in[2].o_texcoord0;
    o_texcoord1 = gs_in[2].o_texcoord1;
    o_texcoord2 = gs_in[2].o_texcoord2;
    EmitVertex();

    EndPrimitive();

    gl_Position = gl_in[2].gl_Position;
    o_position = gs_in[2].o_position;
    o_normal = gs_in[2].o_normal;
    o_tangent = gs_in[2].o_tangent;
    o_bitangent = gs_in[2].o_bitangent;
    o_texcoord0 = gs_in[2].o_texcoord0;
    o_texcoord1 = gs_in[2].o_texcoord1;
    o_texcoord2 = gs_in[2].o_texcoord2;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position;
    o_position = gs_in[0].o_position;
    o_tangent = gs_in[0].o_tangent;
    o_bitangent = gs_in[0].o_bitangent;
    o_texcoord0 = gs_in[0].o_texcoord0;
    o_texcoord1 = gs_in[0].o_texcoord1;
    o_texcoord2 = gs_in[0].o_texcoord2;
    EmitVertex();

    EndPrimitive();
}
