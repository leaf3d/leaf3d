#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat3 normalMatrix;
uniform vec3 lightPos;

in vec3 position;
in vec3 normal;
in vec2 texcoord0;

out vec2 Texcoord0;
out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;

void main() {
    Texcoord0 = texcoord0;
    Normal =  normalize(normalMatrix * normal);
    FragPos = vec3(view * model * vec4(position, 1.0));
    LightPos = vec3(view * vec4(lightPos, 1.0));
    gl_Position = proj * view * model * vec4(position, 1.0);
}
