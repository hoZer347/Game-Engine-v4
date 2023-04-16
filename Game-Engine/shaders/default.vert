#version 450

layout(location = 1) in vec4 pos;
layout(location = 2) in vec4 cds;

out vec2 _cds;

void main() {
    _cds = vec2(cds);

    gl_Position = pos;
};
