#version 450

in vec2 _cdr;

layout (binding = 0) uniform sampler2D tex;

out vec4 fclr;

void main()
{
	fclr = texture(tex, _cdr);
}
