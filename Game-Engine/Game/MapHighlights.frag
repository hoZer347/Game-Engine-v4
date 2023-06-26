#version 450

out vec4 clr;
in float type;

void main()
{
	if		(type < 0.1f)	clr = vec4( 1,  1,  1,  1);	// None
	else if	(type < 1.1f)	clr = vec4(.5, .5,  1, .5);	// Player
	else if (type < 2.1f)	clr = vec4( 1, .5, .5, .5);	// Enemy
	else if (type < 3.1f)	clr = vec4(.5,  1, .5, .5);	// Ally
	else if (type < 4.1f)	clr = vec4( 1, .5, .5, .5);	// Attack Range
	else if (type < 5.1f)	clr = vec4( 1,  1, .5, .5);	// Enemy 2
	else if (type < 6.1f)	clr = vec4(.5,  1,  1, .5);	// Enemy 3
	else					clr = vec4( 1,  0,  1,  1);	// Error
};
