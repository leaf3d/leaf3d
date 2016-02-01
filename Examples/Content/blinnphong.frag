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
    int     type;
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

// Camera position.
uniform vec3        u_cameraPos;

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
    in vec3 normalDirection,
    in vec3 surfaceToLightDirection
)
{
   // Calculation as for Lambertian reflection.
   float diffuseTerm = clamp(dot(normalDirection, surfaceToLightDirection), 0, 1) ;
   return diffuse * color.rgb * color.a * diffuseTerm;
}

// Returns intensity of specular reflection.
vec3 specularLighting(
    in vec3 specular,
    in float shininess,
    in vec4 color,
    in vec3 normalDirection,
    in vec3 surfaceToLightDirection,
    in vec3 surfaceToCameraDirection
)
{
   vec3 halfVector = normalize(surfaceToLightDirection + surfaceToCameraDirection);
   float specularTerm = pow(dot(normalDirection, halfVector), shininess);
   return specular * (color.rgb * color.a * specularTerm);
}

// Returns light attenuation based on distance.
float lightingAttenuation(
    in float kc,
    in float kl,
    in float kq,
    in float surfaceToLightDistance
)
{
    return 1.0f / (kc + kl * surfaceToLightDistance + kq * (surfaceToLightDistance * surfaceToLightDistance));
}

/* MAIN ***********************************************************************/

void main()
{
    // Initial lighting is given by the ambient light.
    vec4 lighting = vec4(ambientLighting(), 1.0f);

    // Iterate over all lights.
    for (int i = 0; i < min(u_lightNr, NR_MAX_LIGHTS); i++)
    {
        vec3    surfaceToLight = u_light[i].position - o_position;
        float   attenuation = 1.0f;

        // Directional light.
        if (u_light[i].type == 0)
            surfaceToLight = -u_light[i].direction;

        // Normalize vectors after interpolation.
        float   surfaceToLightDistance = length(surfaceToLight);
        vec3    surfaceToLightDirection = normalize(surfaceToLight);
        vec3    surfaceToCameraDirection = normalize(u_cameraPos - o_position);
        vec3    lightDirection = normalize(-u_light[i].direction);

        // Point light.
        if (u_light[i].type == 1)
        {
            // Calculate attenuation based on distance.
            attenuation = lightingAttenuation(
                u_light[i].kc,
                u_light[i].kl,
                u_light[i].kq,
                surfaceToLightDistance
            );
        }

        // Spotlight.
        else if (u_light[i].type == 2)
        {
            float theta         = dot(surfaceToLightDirection, lightDirection);
            float epsilon       = u_light[i].kc - u_light[i].kl;
            attenuation         = clamp((theta - u_light[i].kl) / epsilon, 0.0, 1.0);
        }

        // Get Blinn-Phong reflectance components.
        vec3 Idif = diffuseLighting(
            u_material.diffuse,
            u_light[i].color,
            o_normal,
            surfaceToLightDirection
        );
        vec3 Ispe = specularLighting(
            u_material.specular,
            u_material.shininess,
            u_light[i].color,
            o_normal,
            surfaceToLightDirection,
            surfaceToCameraDirection
        );

        lighting += vec4(Idif * attenuation + Ispe * attenuation, 0);
    }

    // Diffuse color of the object from texture.
    vec4 diffuseColor = texture(u_diffuseMap, o_texcoord0);

    // Combination of all components and diffuse color of the object.
    gl_FragColor = diffuseColor * lighting;
}
