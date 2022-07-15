#version 450

layout (location = 0) in vec3 vtx;
layout (location = 1) in vec2 cdr;

layout (location = 0) uniform mat4 mvp;

out vec2 _cdr;

void main()
{
	_cdr = cdr;

	gl_Position = mvp * vec4(vtx, 1);
}
