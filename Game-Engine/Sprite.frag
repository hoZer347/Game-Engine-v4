#version 450

in vec2 _cdr;

layout (binding = 0) uniform sampler2D tex;

void main()
{
	gl_FragColor = vec4(1, 1, 1, 1) * texture(tex, _cdr);
}
