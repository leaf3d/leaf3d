#version 330 core

out vec4 fragColor;

/* STRUCTS ********************************************************************/

struct Material {
    vec3 diffuse;
};

/* UNIFORMS *******************************************************************/

uniform Material u_material;

/* MAIN ***********************************************************************/

void main()
{
    fragColor = vec4(u_material.diffuse, 1);
}
