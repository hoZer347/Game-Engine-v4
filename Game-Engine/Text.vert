#version 450

layout (location = 0) in vec4 vtx;
layout (location = 1) in vec4 cdr;
layout (location = 2) in vec4 clr;
layout (location = 3) in vec4 nrm;

layout (location = 0) uniform mat4 mvp = mat4(1);

out vec2 _cdr;

void main()
{
	_cdr = vec2(cdr);
	gl_Position = mvp * vtx;
}
