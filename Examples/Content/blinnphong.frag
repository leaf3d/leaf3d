#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec4 color;
    float kc;
    float kl;
    float kq;
};

// Data from vertex shader
in vec3 o_position;
in vec3 o_normal;
in vec2 o_texcoord0;

// Diffuse map
uniform sampler2D u_diffuseMap;

// Material and lights
uniform Material u_material;
uniform Light u_light;
uniform vec4 u_ambientColor;

// Returns intensity of reflected ambient lighting
vec3 ambientLighting()
{
   return u_material.ambient * u_ambientColor.xyz * u_ambientColor.w;
}

// Returns intensity of diffuse reflection
vec3 diffuseLighting(in vec3 N, in vec3 L)
{
   // Calculation as for Lambertian reflection
   float diffuseTerm = clamp(dot(N, L), 0, 1) ;
   return u_material.diffuse * u_light.color.xyz * u_light.color.w * diffuseTerm;
}

// Returns intensity of specular reflection
vec3 specularLighting(in vec3 N, in vec3 L, in vec3 V)
{
   // half vector
   vec3 H = normalize(L + V);
   float specularTerm = pow(dot(N, H), u_material.shininess);

   return u_material.specular * (u_light.color.xyz * u_light.color.w * specularTerm);
}

// Returns light attenuation based on distance
float lightingAttenuation(in float D)
{
    return 1.0f / (u_light.kc + u_light.kl * D + u_light.kq * (D * D)); 
}

void main() {
    // Calculate distance between light and eye
    float D = length(u_light.position - o_position);

    // Normalize vectors after interpolation
    vec3 L = normalize(u_light.position - o_position);
    vec3 V = normalize(-o_position);
    vec3 N = normalize(o_normal);

    // Get Blinn-Phong reflectance components
    vec3 Iamb = ambientLighting();
    vec3 Idif = diffuseLighting(N, L);
    vec3 Ispe = specularLighting(N, L, V);

    // Get light attenuation
    float att = lightingAttenuation(D);

    // Diffuse color of the object from texture
    vec4 diffuseColor = texture(u_diffuseMap, o_texcoord0);
    vec4 lighting = vec4(Iamb * att + Idif * att + Ispe * att, 1);

    // Combination of all components and diffuse color of the object
    gl_FragColor = diffuseColor * lighting;
}
