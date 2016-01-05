#version 330 core

// Data from vertex shader
in vec3 o_camera;
in vec3 o_position;
in vec3 o_normal;
in vec2 o_texcoord0;

// Diffuse map
uniform sampler2D u_diffuseMap;

void main() {
    gl_FragColor = texture(u_diffuseMap, o_texcoord0);
}
