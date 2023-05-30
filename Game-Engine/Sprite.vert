#version 450

in  vec4  pos;
out vec4 _pos;


layout(location = 0) uniform mat4 mvp = mat4(1);
layout(location = 1) uniform mat4 trns = mat4(1);


void main()
{
    _pos = pos;
    gl_Position = mvp * trns * pos;
};
