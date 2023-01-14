#version 450

uniform vec4 color = vec4(1);

out vec4 _color;

void main()
{
	_color = color;
};
