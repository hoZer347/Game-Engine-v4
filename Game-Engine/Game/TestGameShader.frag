#version 450

out vec4 clr;

layout (location = 2) uniform vec4 color = vec4(0, 0, 0, 1);

void main()
{
    clr = color;
};
