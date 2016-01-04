#version 330 core

uniform sampler2D albedoMap;

in vec2 Texcoord0;

void main() {
    gl_FragColor = texture(albedoMap, Texcoord0);
}
