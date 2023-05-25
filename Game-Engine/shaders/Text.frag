#version 450

in vec2 _cds;

uniform sampler2D tex;

out vec4 o_clr;

void main()
{
    o_clr = texture(tex, _cds);
};
