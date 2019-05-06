#version 330 core

out vec4 fragColor;

/* STRUCTS ********************************************************************/

struct Material {
    vec3    diffuse;
};

/* INPUTS *********************************************************************/

// Data from vertex shader.
in VertexData {
  vec3      position;
  vec3      normal;
  vec3      tangent;
  vec3      bitangent;
  vec2      texcoord0;
  vec3      diffuseVariation;
  flat int  LOD;
} fs_in;

/* UNIFORMS **************************************************************/

// Diffuse map.
uniform sampler2D   u_diffuseMap;

// Camera position.
uniform vec3        u_cameraPos;
uniform float       u_grassDistanceLOD1;
uniform float       u_grassDistanceLOD2;
uniform float       u_grassDistanceLOD3;

// Material and lights.
uniform Material    u_material;

/* UTILS **********************************************************************/

/* MAIN ***********************************************************************/

void main()
{
    vec4 diffuse = texture(u_diffuseMap, fs_in.texcoord0);
    vec3 diffuseColor = u_material.diffuse + fs_in.diffuseVariation;

    // Discard fragments with low alpha value.
    if (diffuse.a < 0.5f)
      discard;

    /* FOR DEBUG: *
    if (fs_in.LOD == 3)
      diffuseColor = vec3(1, 0, 0);
    else if (fs_in.LOD == 2)
        diffuseColor = vec3(0, 1, 0);
    else if (fs_in.LOD == 1)
        diffuseColor = vec3(0, 0, 1);
    /**/

    // Calculate fade factors.
    float surfaceToCameraDistance = length(fs_in.position - u_cameraPos);
    float alphaOpacity = clamp((u_grassDistanceLOD3 - surfaceToCameraDistance) / (u_grassDistanceLOD3 - u_grassDistanceLOD1), 0, 1);

    // Final fragment color.
    fragColor = vec4(diffuse.rgb * diffuseColor, alphaOpacity);
}
