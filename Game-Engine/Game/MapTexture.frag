#version 450

out vec4 clr;
in vec4 _pos;

uniform sampler2D tex;

void main()
{
	clr = texture(tex, _pos.xy);
};
