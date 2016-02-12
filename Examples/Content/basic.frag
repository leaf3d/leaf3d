#version 330 core

/* STRUCTS ********************************************************************/

struct Material {
    vec3    diffuse;
};

/* UNIFORMS *******************************************************************/

uniform Material u_material;

/* MAIN ***********************************************************************/

void main()
{
    gl_FragColor = vec4(u_material.diffuse, 1);
}
