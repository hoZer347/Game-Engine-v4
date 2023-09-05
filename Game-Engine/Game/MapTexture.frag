#version 450

out vec4 clr;
in vec2 cds;

uniform sampler2D tex;

void main()
{
	clr = texture(tex, cds);
};
