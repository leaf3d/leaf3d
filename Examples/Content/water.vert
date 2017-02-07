#version 330 core

const float pi2 = 6.28318;

/* ATTRIBUTES *****************************************************************/

in vec3 i_position;     // xyz - position
in vec3 i_normal;       // xyz - normal
in vec3 i_tangent;      // xyz - tangent
in vec2 i_texcoord0;    // xy - texture0 coords

/* UNIFORMS *******************************************************************/

// Matrices.
uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;
uniform mat3 u_normalMat;

// Simulation.
uniform float   u_time;
uniform float   u_waterHeight;
uniform int     u_numWaves;
uniform float   u_amplitude[8];
uniform float   u_wavelength[8];
uniform float   u_speed[8];
uniform vec2    u_direction[8];

/* OUTPUTS ********************************************************************/

// Data for fragment shader.
out vec3    o_position;
out vec3    o_normal;
out vec3    o_tangent;
out vec3    o_bitangent;
out vec2    o_texcoord0;

/* WAVE GENERATOR *************************************************************/

float wave(int i, float x, float y)
{
    float frequency = pi2 / u_wavelength[i];
    float phase = u_speed[i] * frequency;
    float theta = dot(u_direction[i], vec2(x, y));
    return u_amplitude[i] * sin(theta * frequency + u_time * phase);
}

float waveHeight(float x, float y)
{
    float height = 0.0;
    for (int i = 0; i < u_numWaves; ++i)
        height += wave(i, x, y);
    return height;
}

float dWavedx(int i, float x, float y) {
    float frequency = pi2 / u_wavelength[i];
    float phase = u_speed[i] * frequency;
    float theta = dot(u_direction[i], vec2(x, y));
    float A = u_amplitude[i] * u_direction[i].x * frequency;
    return A * cos(theta * frequency + u_time * phase);
}

float dWavedy(int i, float x, float y) {
    float frequency = pi2 / u_wavelength[i];
    float phase = u_speed[i] * frequency;
    float theta = dot(u_direction[i], vec2(x, y));
    float A = u_amplitude[i] * u_direction[i].y * frequency;
    return A * cos(theta * frequency + u_time * phase);
}

vec3 waveNormal(float x, float y) {
    float dx = 0.0;
    float dy = 0.0;
    for (int i = 0; i < u_numWaves; ++i) {
        dx += dWavedx(i, x, y);
        dy += dWavedy(i, x, y);
    }
    vec3 n = vec3(-dx, -dy, 1.0);
    return normalize(n);
}

/* MAIN ***********************************************************************/

void main(void)
{
    // Vertex position in world space.
    vec4 worldSpacePosition = u_modelMat * vec4(i_position, 1);
    worldSpacePosition.y = u_waterHeight + waveHeight(worldSpacePosition.x, worldSpacePosition.z);
    o_position = worldSpacePosition.xyz / worldSpacePosition.w;

    // Normal in world space.
    o_normal	= normalize(u_normalMat * waveNormal(o_position.x, o_position.y));

    // Tangent in world space.
    o_tangent	= normalize(u_normalMat * i_tangent);
    // Re-orthogonalize tangent with respect to normal
    //o_tangent = normalize(o_tangent - dot(o_tangent, o_normal) * o_normal);

    // Bi-tagent in world space.
    o_bitangent = cross(o_tangent, o_normal);

    // Texture coordinates to fragment shader.
    o_texcoord0	= i_texcoord0;

    // Vertex position in screen space.
    gl_Position = u_projMat * u_viewMat * worldSpacePosition;
}
