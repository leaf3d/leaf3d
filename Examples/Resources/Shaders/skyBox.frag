#version 330 core

out vec4 fragColor;

/* INPUTS *********************************************************************/

// Data from vertex shader.
// Direction vector representing a 3D texture coordinate.
in VertexData {
   vec3 textureDir;
} fs_in;

/* UNIFORMS *******************************************************************/

// Cubemap texture sampler.
uniform samplerCube u_cubemap;

/* MAIN ***********************************************************************/

void main()
{
    // Final fragment color.
    fragColor = texture(u_cubemap, fs_in.textureDir);
}
