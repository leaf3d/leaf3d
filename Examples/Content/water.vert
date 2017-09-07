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
uniform mat4 u_vpMat;
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
out VertexData {
  vec3    position;
  vec3    normal;
  vec3    tangent;
  vec3    bitangent;
  vec2    texcoord0;
  vec2    texcoord1;
} vs_out;

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
    vs_out.position = worldSpacePosition.xyz / worldSpacePosition.w;

    // Normal in world space.
    vs_out.normal	= normalize(u_normalMat * waveNormal(vs_out.position.x, vs_out.position.y));

    // Tangent in world space.
    vs_out.tangent	= normalize(u_normalMat * i_tangent);
    // Re-orthogonalize tangent with respect to normal
    vs_out.tangent = normalize(vs_out.tangent - dot(vs_out.tangent, vs_out.normal) * vs_out.normal);

    // Bi-tagent in world space.
    vs_out.bitangent = cross(vs_out.tangent, vs_out.normal);

    // Texture coordinates to fragment shader.
    float d0 = u_time * -0.02f;
    float d1 = u_time * 0.05f;
    float d2 = u_time * 0.03f;
    vs_out.texcoord0	= i_texcoord0 + vec2(d0, d1);
    vs_out.texcoord1	= i_texcoord0 * mat2x2(cos(0.5f), -sin(0.5f), sin(0.5f), cos(0.5f)) + vec2(d1, d0);

    // Vertex position in screen space.
    gl_Position = u_vpMat * worldSpacePosition;
}
