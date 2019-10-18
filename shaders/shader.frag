#version 330 core
// This is a sample fragment shader.

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 1

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 normalOutput;
in vec3 posOutput;

uniform vec3 color;
uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLights[NR_POINT_LIGHTS];

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    
    vec3 norm = normalize(normalOutput);
    vec3 viewDir = normalize(viewPos - posOutput);
    
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    fragColor = vec4(color, sampleExtraOutput);
}

// Calculates the color when using the point light
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Diffuse shading
    float diffu = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 rflctDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, rflctDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPosition);
    float attenuation = 1.0f / (distance * light.linear);
    
    // Combine things
    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diffu * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    vec3 result = ambient + diffuse + specular;
    return result
}
