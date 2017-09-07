#version 330 core

#define M_PI  3.1415926535897932384626433832795
#define M_PI2 6.2831853071795864769252867665595

layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;

/* CONSTS *********************************************************************/

/* INPUTS *********************************************************************/

in VertexData {
    vec3 position;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    vec2 texcoord0;
} gs_in[];

/* UNIFORMS *******************************************************************/

uniform mat4 u_vpMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;

uniform vec3  u_cameraPos;
uniform float u_grassDistanceLOD1;
uniform float u_grassDistanceLOD2;
uniform float u_grassDistanceLOD3;
uniform float u_grassHeight;
uniform float u_grassHeightVariation;

/* OUTPUTS ********************************************************************/

out VertexData {
    vec3 position;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    vec2 texcoord0;
} gs_out;

/* UTILS **********************************************************************/

mat3 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat3(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c          );
}

float rand(vec2 seed)
{
    return fract(sin(dot(seed.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void makeQuad(vec3 p1, vec3 p2, vec3 n1, vec3 n2, float height, mat4 vpMat)
{
    vec3 pos = (p1 + p2) * 0.5;
    vec3 forward = normalize(pos);

    float width = distance(p1, p2);
    float randK = rand(pos.xy + p1.xy);
    float angle = randK * M_PI;
    float randWidth = width * (1 + randK);
    float randHeight = height * (1 + randK * u_grassHeightVariation);

    vec3 up = normalize((n1 + n2) * 0.5);
    vec3 right = normalize(rotationMatrix(up, angle) * cross(forward, up));
    vec3 normal = normalize(cross(right, up));

    vec4 v1 = vec4(pos + right * 0.5 * randWidth + up * 0 * randHeight, 1.0);
    vec4 v2 = vec4(pos + right * 0.5 * randWidth + up * 1 * randHeight, 1.0);
    vec4 v3 = vec4(pos + right * -0.5 * randWidth + up * 0 * randHeight, 1.0);
    vec4 v4 = vec4(pos + right * -0.5 * randWidth + up * 1 * randHeight, 1.0);

    v2.x += randK - 0.5;
    v4.z += randK - 0.5;

    gl_Position = vpMat * v1;
    gs_out.position = v1.xyz / v1.w;
    gs_out.normal = normal;
    gs_out.tangent = right;
    gs_out.bitangent = up;
    gs_out.texcoord0 = vec2(0, 1);
    EmitVertex();

    gl_Position = vpMat * v2;
    gs_out.position = v2.xyz / v2.w;
    gs_out.normal = normal;
    gs_out.tangent = right;
    gs_out.bitangent = up;
    gs_out.texcoord0 = vec2(0, 0);
    EmitVertex();

    gl_Position = vpMat * v3;
    gs_out.position = v3.xyz / v3.w;
    gs_out.normal = normal;
    gs_out.tangent = right;
    gs_out.bitangent = up;
    gs_out.texcoord0 = vec2(1, 1);
    EmitVertex();

    gl_Position = vpMat * v4;
    gs_out.position = v4.xyz / v4.w;
    gs_out.normal = normal;
    gs_out.tangent = right;
    gs_out.bitangent = up;
    gs_out.texcoord0 = vec2(1, 0);
    EmitVertex();

    EndPrimitive();
}

/* MAIN ***********************************************************************/

void main()
{
    if (u_grassHeight > 0)
    {
        vec3 p1 = gs_in[0].position;
        vec3 p2 = gs_in[1].position;
        vec3 p3 = gs_in[2].position;

        vec3 n1 = gs_in[0].normal;
        vec3 n2 = gs_in[1].normal;
        vec3 n3 = gs_in[2].normal;

        vec3 center = (p1 + p2 + p3) * 0.33333;
        vec3 cameraLookAt = vec3(u_vpMat[2][0], u_vpMat[2][1], u_vpMat[2][2]);
        vec3 surfaceToCamera = center - u_cameraPos;
        float surfaceToCameraDistance = length(surfaceToCamera);
        bool inFront = (dot(surfaceToCamera, cameraLookAt) > 0);

        if (inFront && surfaceToCameraDistance < u_grassDistanceLOD3)
        {
            makeQuad(p1, p2, n1, n2, u_grassHeight, u_vpMat);

            if (surfaceToCameraDistance < u_grassDistanceLOD2)
            {
                makeQuad(p2, p3, n2, n3, u_grassHeight, u_vpMat);

                if (surfaceToCameraDistance < u_grassDistanceLOD1)
                {
                    makeQuad(p3, p1, n3, n1, u_grassHeight, u_vpMat);
                }
            }
        }
    }
}