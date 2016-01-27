#version 330 core

#define NR_MAX_LIGHTS 16

/* STRUCTS ********************************************************************/

struct Material {
    vec3    ambient;
    vec3    diffuse;
    vec3    specular;
    float   shininess;
};

struct Light {
    vec3    position;
    vec3    direction;
    vec4    color;
    float   kc;
    float   kl;
    float   kq;
};

/* INPUTS *********************************************************************/

// Data from vertex shader.
in vec3 o_position;
in vec3 o_normal;
in vec2 o_texcoord0;

/* UNIFORMS *******************************************************************/

// Diffuse map.
uniform sampler2D   u_diffuseMap;

// Material and lights.
uniform Material    u_material;
uniform int         u_lightNr;
uniform Light       u_light[NR_MAX_LIGHTS];
uniform vec4        u_ambientColor;

/* UTILS **********************************************************************/

// Returns intensity of reflected ambient lighting.
vec3 ambientLighting()
{
   return u_material.ambient * u_ambientColor.xyz * u_ambientColor.w;
}

// Returns intensity of diffuse reflection.
vec3 diffuseLighting(
    in vec3 diffuse,
    in vec4 color,
    in vec3 N,
    in vec3 L
)
{
   // Calculation as for Lambertian reflection.
   float diffuseTerm = clamp(dot(N, L), 0, 1) ;
   return diffuse * color.xyz * color.w * diffuseTerm;
}

// Returns intensity of specular reflection.
vec3 specularLighting(
    in vec3 specular,
    in float shininess,
    in vec4 color,
    in vec3 N,
    in vec3 L,
    in vec3 V
)
{
   // Half vector
   vec3 H = normalize(L + V);
   float specularTerm = pow(dot(N, H), shininess);
   return specular * (color.xyz * color.w * specularTerm);
}

// Returns light attenuation based on distance.
float lightingAttenuation(
    in float kc,
    in float kl,
    in float kq,
    in float D
)
{
    return 1.0f / (kc + kl * D + kq * (D * D));
}

/* MAIN ***********************************************************************/

void main() {
    // Initial lighting is given by the ambient light.
    vec4 lighting = vec4(ambientLighting(), 1.0f);
    
    // Iterate over all lights.
    for (int i = 0; i < u_lightNr; i++)
    {
        // Calculate distance between light and eye.
        float D = length(u_light[i].position - o_position);

        // Normalize vectors after interpolation.
        vec3 L = normalize(u_light[i].position - o_position);
        vec3 V = normalize(-o_position);
        vec3 N = normalize(o_normal);

        // Get Blinn-Phong reflectance components.
        vec3 Idif = diffuseLighting(u_material.diffuse, u_light[i].color, N, L);
        vec3 Ispe = specularLighting(u_material.specular, u_material.shininess, u_light[i].color, N, L, V);

        // Get light attenuation.
        float att = lightingAttenuation(u_light[i].kc, u_light[i].kl, u_light[i].kq, D);

        lighting += vec4(Idif * att + Ispe * att, 0);
    }

    // Diffuse color of the object from texture.
    vec4 diffuseColor = texture(u_diffuseMap, o_texcoord0);

    // Combination of all components and diffuse color of the object.
    gl_FragColor = diffuseColor * lighting;
}
