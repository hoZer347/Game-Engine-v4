#version 450

in  vec4 _pos;
out vec4  clr;

layout (location = 2) uniform vec4 loc = vec4(1);

layout (binding = 0) uniform sampler2D tex;

// Remember we are aligning to position and xz / xy

void main()
{
	clr = texture(tex, loc.xy + _pos.xz * loc.zw);
};
