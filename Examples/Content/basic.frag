#version 330 core

// Flat object color
uniform vec4 u_objectColor;

void main() {
    gl_FragColor = u_objectColor;
}
