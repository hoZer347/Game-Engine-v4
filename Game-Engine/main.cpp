// Debug
#include "Mem_Leak.h"
//

// OpenGL
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
using namespace glm;
//

// STD includes
#include <iostream>
//

// Local Includes
#include "Engine.h"
#include "Thread.h"
#include "Camera.h"
#include "Data.h"
using namespace eng;
//

int main()
{
	open();

	auto w0 = wnd::open();
	auto w1 = wnd::open();
	auto w2 = wnd::open();

	RenderObj* r;

	Matl matl = {
		{ "Sprite" },
		{ { "Resources/DK.png", GL_RGBA } },
	};
	Mesh mesh =
	{
		{
			{ vec4(0, 0, -5, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(0, 0, 0, 1) },
			{ vec4(1, 0, -5, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(1, 0, 0, 1) },
			{ vec4(1, 1, -5, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(1, 1, 0, 1) },
			{ vec4(0, 1, -5, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(0, 1, 0, 1) },
		},  { 0, 1, 2, 3 }
	};

	wnd::bind(r, w0, matl, mesh, GL_QUADS);

	input::open(w0);
	input::open(w1);
	input::open(w2);

	input::next();

	input::bind([]()
		{
			std::cout << "Test1" << std::endl;

			return false;
		}, []()
		{
			std::cout << "Test2" << std::endl;

			return false;
		}, GLFW_MOUSE_BUTTON_1, 0);

	while (w0)
	{
		
	};

	close();

	return 0;
};
