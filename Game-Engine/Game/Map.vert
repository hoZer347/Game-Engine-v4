#version 450

in vec4 pos;

uniform mat4 mvp = mat4(1);

void main()
{
	gl_Position = mvp * pos;
};