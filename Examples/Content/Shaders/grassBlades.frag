#version 330 core

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
  float     distanceK;
  flat int  LOD;
} fs_in;

/* UNIFORMS **************************************************************/

// Diffuse map.
uniform sampler2D   u_diffuseMap;

// Camera position.
uniform vec3        u_cameraPos;

// Material and lights.
uniform Material    u_material;

/* UTILS **********************************************************************/

/* MAIN ***********************************************************************/

void main()
{
    vec4 diffuse = texture(u_diffuseMap, fs_in.texcoord0);
    vec3 diffuseColor = u_material.diffuse;

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

    // Calculate fog factor.
    float surfaceToCameraDistance = length(fs_in.position - u_cameraPos);

    // Final fragment color.
    gl_FragColor = vec4(diffuse.rgb * diffuseColor, fs_in.distanceK);
}
