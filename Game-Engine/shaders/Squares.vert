#version 450

layout (location = 1) in vec4 pos;
layout (location = 2) in vec2 cds;

uniform mat4 trns = mat4(1);
uniform mat4 mvp  = mat4(1);

out vec2 _cds;

void main()
{
	_cds = cds;
	gl_Position = mvp * trns * pos;
};
