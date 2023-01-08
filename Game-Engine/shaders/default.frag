#version 450

in vec4 _clr;
in vec4 _cds;

layout(binding = 0) uniform sampler2D tex;

out vec4 o_clr;

void main() {
    o_clr = _clr * texture(tex, vec2(_cds));
};
