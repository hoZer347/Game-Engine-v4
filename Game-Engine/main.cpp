// Debug
#include "Mem_Leak.h"
//

// OpenGL
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;
//

// STD includes
#include <iostream>
//

// Local Includes
#include "Engine.h"
#include "Thread.h"
#include "Data.h"
using namespace eng;
//

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	open();

	auto w0 = wnd::open();

	Mesh mesh =
	{
		
	};

	Mesh mesh1 =
	{
		{
			{ vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(0, 0, 0, 1), },
			{ vec4(1, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(1, 0, 0, 1), },
			{ vec4(1, 1, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(1, 1, 0, 1), },
			{ vec4(0, 1, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(0, 1, 0, 1), },
		},  { 0, 1, 2, 3 }
	};

	Mesh mesh2 =
	{
		{
			{ vec4( 0,  0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(0, 0, 0, 1), },
			{ vec4(-1,  0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(1, 0, 0, 1), },
			{ vec4(-1, -1, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(1, 1, 0, 1), },
			{ vec4( 0, -1, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(0, 1, 0, 1), },
		},  { 0, 1, 2, 3 }
	};

	Matl matl =
	{
		{ "Sprite" },
		{ { "Resources/DK.png", GL_RGBA } },
	};

	wnd::bind(w0, matl, mesh, GL_QUADS);
	Thread<2>::init();

	while (true)
	{
		mesh = mesh1;
		mesh = mesh2;
	};

	close();

	return 0;
};
