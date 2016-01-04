#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

in vec3 position;
in vec3 normal;
in vec2 texcoord0;

out vec2 Texcoord0;

void main() {
    Texcoord0 = texcoord0;
    gl_Position = proj * view * model * vec4(position, 1.0);
}
