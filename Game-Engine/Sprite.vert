#version 450

in vec4 vtx;
in vec4 cdr;
in vec4 clr;
in vec4 nrm;

layout (location = 0) uniform mat4 mvp = mat4(1);

out vec2 _cdr;


void main()
{
	_cdr = vec2(cdr);

	gl_Position = vtx;
}
