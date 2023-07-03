#version 450

layout(points) in;
layout(triangle_strip, max_vertices=3) out;

out vec2 cds;

void main()
{
	gl_Position = gl_in[0].gl_Position;
	cds = gl_in[0].gl_Position.xy;
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	cds = gl_in[1].gl_Position.xy;
	EmitVertex();
    
	gl_Position = gl_in[2].gl_Position;
	cds = gl_in[2].gl_Position.xy;
	EmitVertex();

	EndPrimitive();
};
