#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

/* INPUTS *********************************************************************/

in VertexData {
    vec3    position;
    vec3    normal;
    vec3    tangent;
    vec3    bitangent;
    vec2    texcoord0;
} gs_in[];

/* OUTPUTS ********************************************************************/

out VertexData {
    vec3    position;
    vec3    normal;
    vec3    tangent;
    vec3    bitangent;
    vec2    texcoord0;
} gs_out;

/* MAIN ***********************************************************************/

void main() {
    gl_Position = gl_in[0].gl_Position;
    gs_out.position = gs_in[0].position;
    gs_out.tangent = gs_in[0].tangent;
    gs_out.bitangent = gs_in[0].bitangent;
    gs_out.texcoord0 = gs_in[0].texcoord0;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    gs_out.position = gs_in[1].position;
    gs_out.normal = gs_in[1].normal;
    gs_out.tangent = gs_in[1].tangent;
    gs_out.bitangent = gs_in[1].bitangent;
    gs_out.texcoord0 = gs_in[1].texcoord0;
    EmitVertex();

    EndPrimitive();

    gl_Position = gl_in[1].gl_Position;
    gs_out.position = gs_in[1].position;
    gs_out.normal = gs_in[1].normal;
    gs_out.tangent = gs_in[1].tangent;
    gs_out.bitangent = gs_in[1].bitangent;
    gs_out.texcoord0 = gs_in[1].texcoord0;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    gs_out.position = gs_in[2].position;
    gs_out.normal = gs_in[2].normal;
    gs_out.tangent = gs_in[2].tangent;
    gs_out.bitangent = gs_in[2].bitangent;
    gs_out.texcoord0 = gs_in[2].texcoord0;
    EmitVertex();

    EndPrimitive();

    gl_Position = gl_in[2].gl_Position;
    gs_out.position = gs_in[2].position;
    gs_out.normal = gs_in[2].normal;
    gs_out.tangent = gs_in[2].tangent;
    gs_out.bitangent = gs_in[2].bitangent;
    gs_out.texcoord0 = gs_in[2].texcoord0;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position;
    gs_out.position = gs_in[0].position;
    gs_out.tangent = gs_in[0].tangent;
    gs_out.bitangent = gs_in[0].bitangent;
    gs_out.texcoord0 = gs_in[0].texcoord0;
    EmitVertex();

    EndPrimitive();
}
