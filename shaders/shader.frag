#version 330 core
// This is a sample fragment shader.

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 color;
};

#define NR_POINT_LIGHTS 1
#define NR_MATERIALS 3

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 normalOutput;
in vec3 posOutput;

uniform vec3 color;
uniform vec3 viewPos;
uniform int matType;
uniform bool normalColoring;
uniform Material material[NR_MATERIALS];
uniform PointLight pointLights[NR_POINT_LIGHTS];

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    
    vec3 norm = normalize(normalOutput);
    vec3 normColor = (norm + vec3(1,1,1)) / 2;
    vec3 viewDir = normalize(viewPos - posOutput);
    
    vec3 result = vec3(0,0,0);
    
    // Point lights calculations
    if(!normalColoring)
        for(int i = 0; i < NR_POINT_LIGHTS; i++)
            result += CalcPointLight(pointLights[i], norm, posOutput, viewDir);
    else
        result = normColor;
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    fragColor = vec4(result, sampleExtraOutput);
}

// Calculates the color when using the point light
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    
    float ambInt = .1f;
    
    // Diffuse shading
    float diffu = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 rflctDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, rflctDir), 0.0), material[matType].shininess);
    // Attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0f / (dist * dist * light.quadratic);
    
    // Combine things
    vec3 ambient = ambInt * material[matType].ambient;
    vec3 diffuse = material[matType].diffuse * diffu;
    vec3 specular = material[matType].specular * spec;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    vec3 result = ambient + diffuse + specular;
    return result * light.color;
}
