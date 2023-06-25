#version 450

in vec4 pos;
out vec4 _pos;

uniform mat4 mvp = mat4(1);

void main()
{
	_pos = pos;
	gl_Position = mvp * pos;
};
