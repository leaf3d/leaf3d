#version 330 core

/* ATTRIBUTES *****************************************************************/

in vec3 i_position;     // xyz - position
in vec3 i_normal;       // xyz - normal
in vec3 i_tangent;      // xyz - tangent
in vec2 i_texcoord0;    // xy - texture0 coords

/* UNIFORMS *******************************************************************/

// Matrices.
uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;
uniform mat3 u_normalMat;

/* OUTPUTS ********************************************************************/

// Data for fragment shader.
out vec3    o_position;
out vec3    o_normal;
out vec3    o_tangent;
out vec3    o_bitangent;
out vec2    o_texcoord0;

/* MAIN ***********************************************************************/

void main(void)
{
    // Vertex position in world space.
    vec4 worldSpacePosition = u_modelMat * vec4(i_position, 1);
    o_position = worldSpacePosition.xyz / worldSpacePosition.w;

    // Normal in world space.
    o_normal	= normalize(u_normalMat * i_normal);

    // Tangent in world space.
    o_tangent	= normalize(u_normalMat * i_tangent);
    // Re-orthogonalize tangent with respect to normal
    o_tangent = normalize(o_tangent - dot(o_tangent, o_normal) * o_normal);

    // Bi-tagent in world space.
    o_bitangent = cross(o_tangent, o_normal);

    // Texture coordinates to fragment shader.
    o_texcoord0	= i_texcoord0;

    // Vertex position in screen space.
    gl_Position	= u_projMat * u_viewMat * worldSpacePosition;
}
