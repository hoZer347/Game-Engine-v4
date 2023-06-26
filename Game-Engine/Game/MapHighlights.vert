#version 450

in vec4 pos;
out float type;

uniform mat4 mvp = mat4(1);

void main()
{
	type = pos.w;
	gl_Position = mvp * vec4(pos.xyz, 1);
};
