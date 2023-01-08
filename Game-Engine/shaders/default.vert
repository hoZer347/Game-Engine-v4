#version 450

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 clr;
layout(location = 2) in vec4 nrm;
layout(location = 3) in vec4 cds;

layout(location = 4) uniform mat4 trns = mat4(1);
layout(location = 5) uniform mat4 mvp = mat4(1);

out vec4 _clr;
out vec4 _cds;

void main() {
    _clr = clr;
    _cds = cds;

    gl_Position = trns * mvp * pos;
};
