#version 450

layout(location = 0) in vec4 pos;

layout(location = 0) uniform mat4 trns = mat4(1);
layout(location = 1) uniform mat4 mvp = mat4(1);

void main()
{
    gl_Position = mvp * trns * pos;
};
