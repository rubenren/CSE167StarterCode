#version 330 core
out vec4 FragColor;

in vec3 normalOutput;
in vec3 posOutput;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(posOutput - cameraPos);
    vec3 R = reflect(I, normalize(normalOutput));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
