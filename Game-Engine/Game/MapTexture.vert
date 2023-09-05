#version 450

in vec4 pos;
out vec2 cds;

uniform mat4 mvp = mat4(1);

void main()
{
	cds = pos.xy;
	gl_Position = mvp * vec4(pos.xyz, 1);
};
