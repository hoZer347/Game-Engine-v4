#version 450

in vec2 _cds;

layout(binding = 0) uniform sampler2D tex;

out vec4 clr;

void main()
{
	clr = texture(tex, _cds);
};
