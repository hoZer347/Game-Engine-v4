#version 450

// VV Layout is important here VV
layout(location = 1) in vec4 pos;
layout(location = 2) in vec4 cds;
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

uniform mat4 mvp;
uniform mat4 trns;

out vec2 _cds;

void main() {
    _cds = vec2(cds);

    gl_Position = mvp * trns * pos;
};
