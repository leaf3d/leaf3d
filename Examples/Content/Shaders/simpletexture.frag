#version 330 core

/* INPUTS *********************************************************************/

// Data from vertex shader.
in VertexData {
  vec3 position;
  vec3 normal;
  vec3 tangent;
  vec3 bitangent;
  vec2 texcoord0;
} fs_in;

/* UNIFORMS *******************************************************************/

// Flags.
uniform bool        u_alphaMapEnabled;

// Maps.
uniform sampler2D   u_diffuseMap;
uniform sampler2D   u_alphaMap;

/* MAIN ***********************************************************************/

void main()
{
    vec4 diffuse = texture(u_diffuseMap, fs_in.texcoord0);

    if (u_alphaMapEnabled)
        diffuse.a *= texture(u_alphaMap, fs_in.texcoord0).x;

    if (diffuse.a < 0.1f)
        discard;

    gl_FragColor = diffuse;
}
