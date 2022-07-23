#version 450

in vec4 _clr;
in vec2 _cdr;

layout (binding = 0) uniform sampler2D tex;

out vec4 fclr;

void main()
{
	fclr = texture(tex, _cdr);
}
