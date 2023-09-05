#version 450

out vec4 clr;

uniform vec4 color = vec4(1, 0, 1, 1);

void main()
{
	clr = color;
};
