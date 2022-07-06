#version 450

in vec3 vtx;
in vec2 cdr;

out vec2 _cdr;

void main()
{
	_cdr = cdr;

	gl_Position = vec4(vtx, 1);
}
