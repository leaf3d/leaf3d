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

/* UNIFORMS **************************************************************/

// Diffuse map.
uniform sampler2D   u_diffuseMap;
uniform sampler2D   u_billboardMap;

// Camera position.
uniform vec3        u_cameraPos;

// Material and lights.
uniform Material    u_material;
uniform float       u_grassDistanceLOD1;
uniform float       u_grassDistanceLOD2;
uniform float       u_grassDistanceLOD3;

/* UTILS **********************************************************************/

/* MAIN ***********************************************************************/

void main()
{
    vec4 diffuse = texture(u_diffuseMap, fs_in.texcoord0);

    diffuse.rgb = diffuse.rgb * u_material.diffuse;

    // Discard fragments with low alpha value.
    if (diffuse.a < 0.5f)
      discard;

    // Calculate fog factor.
    float surfaceToCameraDistance = length(fs_in.position - u_cameraPos);
    float alphaOpacity = (u_grassDistanceLOD3 - surfaceToCameraDistance) / (u_grassDistanceLOD3 - u_grassDistanceLOD2);

    // Final fragment color.
    gl_FragColor = vec4(diffuse.rgb, alphaOpacity);
}
