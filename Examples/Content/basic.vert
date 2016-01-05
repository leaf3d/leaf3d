#version 330 core

// Attributes
in vec3 i_position;     // xyz - position
in vec3 i_normal;       // xyz - normal
in vec2 i_texcoord0;    // xy - texture coords

// Matrices
uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;
uniform mat3 u_normalMat;

// Data for fragment shader
out vec3    o_camera;
out vec3    o_position;
out vec3    o_normal;
out vec2    o_texcoord0;

///////////////////////////////////////////////////////////////////

void main(void)
{
   // Position in world space
   vec4 viewSpacePosition = u_viewMat * u_modelMat * vec4(i_position, 1);
   o_position = viewSpacePosition.xyz / viewSpacePosition.w;

   // Normal in world space
   o_normal	= normalize(u_normalMat * i_normal);

   // Camera coordinates to fragment shader
   o_camera = -u_viewMat[3].xyz * mat3(u_viewMat);

   // Texture coordinates to fragment shader
   o_texcoord0	= i_texcoord0;

   // Screen space coordinates of the vertex
   gl_Position	= u_projMat * viewSpacePosition;
}
