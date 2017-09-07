#version 330 core

/* ATTRIBUTES *****************************************************************/

in vec3 i_position;     // xyz - position
in vec3 i_normal;       // xyz - normal
in vec3 i_tangent;      // xyz - tangent
in vec2 i_texcoord0;    // xy - texture0 coords

/* UNIFORMS *******************************************************************/

// Matrices.
uniform mat4 u_modelMat;
uniform mat4 u_vpMat;
uniform mat3 u_normalMat;

/* OUTPUTS ********************************************************************/

// Data for fragment shader.
out VertexData {
  vec3    position;
  vec3    normal;
  vec3    tangent;
  vec3    bitangent;
  vec2    texcoord0;
} vs_out;

/* MAIN ***********************************************************************/

void main(void)
{
    // Vertex position in world space.
    vec4 worldSpacePosition = u_modelMat * vec4(i_position, 1);
    vs_out.position = worldSpacePosition.xyz / worldSpacePosition.w;

    // Normal in world space.
    vs_out.normal	= normalize(u_normalMat * i_normal);

    // Tangent in world space.
    vs_out.tangent	= normalize(u_normalMat * i_tangent);
    // Re-orthogonalize tangent with respect to normal
    vs_out.tangent = normalize(vs_out.tangent - dot(vs_out.tangent, vs_out.normal) * vs_out.normal);

    // Bi-tagent in world space.
    vs_out.bitangent = cross(vs_out.tangent, vs_out.normal);

    // Texture coordinates to fragment shader.
    vs_out.texcoord0	= i_texcoord0;

    // Vertex position in screen space.
    gl_Position	= u_vpMat * worldSpacePosition;
}
