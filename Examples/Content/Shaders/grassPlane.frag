#version 330 core

/* STRUCTS ********************************************************************/

struct Material {
    vec3    diffuse;
};

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
uniform sampler2D   u_diffuseMap;

// Camera position.
uniform vec3        u_cameraPos;

// Material and lights.
uniform Material    u_material;
uniform float       u_grassDistanceLOD3;
uniform float       u_grassDistanceLOD2;
uniform float       u_grassDistanceLOD1;

/* MAIN ***********************************************************************/

void main()
{
    // Calculate diffuse color.
    vec4 diffuse = texture(u_diffuseMap, fs_in.texcoord0);

    // Calculate fog factor.
    float surfaceToCenterDistance = length(fs_in.position);
    float alphaOpacity = (u_grassDistanceLOD3 - surfaceToCenterDistance) / (u_grassDistanceLOD3 - u_grassDistanceLOD2);

    // Calculate final color.
    gl_FragColor = vec4(diffuse.rgb * u_material.diffuse, alphaOpacity);
}
