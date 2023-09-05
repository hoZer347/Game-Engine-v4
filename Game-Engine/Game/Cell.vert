#version 450

in layout(location = 1) vec4 pos;
in layout(location = 2) vec4 cds;

uniform mat4 mvp = mat4(1);
uniform mat4 trns = mat4(1);

void main()
{
	gl_Position = mvp * trns * pos;
};
