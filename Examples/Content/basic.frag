#version 330 core

/* UNIFORMS *******************************************************************/

// Flat object color.
uniform vec4 u_objectColor;

/* MAIN ***********************************************************************/

void main()
{
    gl_FragColor = u_objectColor;
}
