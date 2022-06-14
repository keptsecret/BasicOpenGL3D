#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// controls colors for each aspect of material
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

// controls strength (and color) of aspects of the light source
struct Light {
    int type;           // 0 for directional light, 1 for point light, 2 for spot light
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main() {
    // a simple form of global illumination, simulating ambient light
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));

    // calculate normal and light direction to fragment, now with directional lights and point light attenuation
    vec3 normal = normalize(Normal);
    vec3 lightDir;
    float attenuation = 1.0;
    float theta = 10.0;
    if (light.type == 0) {      // directional light
        lightDir = normalize(-light.direction);
    } else if (light.type == 1) {   // point light
        lightDir = normalize(light.position - FragPos);
        float distance = length(light.position - FragPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    } else if (light.type == 2) {   // spot light
        lightDir = normalize(light.position - FragPos);
        theta = dot(lightDir, normalize(-light.direction));

        float distance = length(light.position - FragPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    }

    // values for softening spotlight
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    // use for diffuse lighting value
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;

    // calculate direction to camera and reflect direction from fragment
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    // apply specular highlights
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), 32.0);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;

    // apply attenuation and softening
    if (light.type != 0) {
        ambient *= attenuation; 
        diffuse *= attenuation * intensity;
        specular *= attenuation * intensity;
    }

    if (light.type == 2) {
        diffuse *= intensity;
        specular *= intensity;
    }

    vec3 finalColor = ambient + diffuse + specular;

    FragColor = vec4(finalColor, 1.0);
}