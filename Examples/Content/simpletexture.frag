#version 330 core

/* INPUTS *********************************************************************/

// Data from vertex shader.
in vec3 o_camera;
in vec3 o_position;
in vec3 o_normal;
in vec2 o_texcoord0;

/* UNIFORMS *******************************************************************/

// Diffuse map.
uniform sampler2D u_diffuseMap;

/* MAIN ***********************************************************************/

void main()
{
    gl_FragColor = texture(u_diffuseMap, o_texcoord0);
}
