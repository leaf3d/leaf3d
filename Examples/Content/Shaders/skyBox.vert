#version 330 core

/* ATTRIBUTES *****************************************************************/

in vec3 i_position;     // xyz - position

/* UNIFORMS *******************************************************************/

// Matrices.
uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;
uniform mat3 u_normalMat;

/* OUTPUTS ********************************************************************/

// Data for fragment shader.
out VertexData {
  vec3    textureDir;
} vs_out;

/* MAIN ***********************************************************************/

void main()
{
    vec4 pos = u_projMat * mat4(mat3(u_viewMat)) * vec4(i_position, 1.0);
    gl_Position = pos.xyww;

    vs_out.textureDir = i_position;
}
