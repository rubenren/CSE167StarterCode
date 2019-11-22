#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec3 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 inColor;

void main()
{
    color = normalize(aCol + inColor);
    vec4 pos = projection * view * model * vec4(aPos, 1.0);
    gl_Position = pos;
}
