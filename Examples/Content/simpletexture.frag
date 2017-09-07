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

// Diffuse map.
uniform sampler2D u_diffuseMap;

/* MAIN ***********************************************************************/

void main()
{
    gl_FragColor = texture(u_diffuseMap, fs_in.texcoord0);
}
