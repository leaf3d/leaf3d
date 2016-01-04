#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform sampler2D albedoMap;
uniform vec4 ambientColor;
uniform vec4 lightColor;
uniform Material material;

in vec2 Texcoord0;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

void main() {
    vec3 lightDir = normalize(LightPos - FragPos);

    vec3 ambientComponent = vec3(ambientColor) * ambientColor.w * material.ambient;

    float diff = max(dot(Normal, lightDir), 0.0);
    vec4 diffuseColor = diff * lightColor;
    vec3 diffuseComponent = vec3(diffuseColor) * diffuseColor.w * material.diffuse;

    vec3 viewDir = normalize(vec3(-FragPos));
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), material.shininess);
    vec3 specularComponent = vec3(lightColor * spec) * material.specular;

    vec4 finalColor = vec4(ambientComponent + diffuseComponent + specularComponent, 1.0f);
    gl_FragColor = vec4(texture(albedoMap, Texcoord0)) * finalColor;
}
