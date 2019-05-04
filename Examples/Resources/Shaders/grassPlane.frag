#version 330 core

out vec4 fragColor;

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
uniform sampler2D   u_dirtMap;

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
    // Calculate distance from camera.
    float surfaceToCameraDistance = length(fs_in.position - u_cameraPos);

    // Calculate fade factors.
    float alphaOpacity = clamp((u_grassDistanceLOD3 - surfaceToCameraDistance) / (u_grassDistanceLOD3 - u_grassDistanceLOD2), 0, 1);
    float grassToDirt = clamp((u_grassDistanceLOD1 - surfaceToCameraDistance) / u_grassDistanceLOD1, 0, 1);

    // Calculate diffuse color.
    vec4 diffuse = texture(u_diffuseMap, fs_in.texcoord0) * vec4(u_material.diffuse, 1);
    vec4 dirt = texture(u_dirtMap, fs_in.texcoord0);

    // Calculate final color.
    fragColor = vec4(mix(diffuse, dirt, grassToDirt).rgb, alphaOpacity);
}
