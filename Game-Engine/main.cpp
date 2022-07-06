// Debug
#include "Mem_Leak.h"
//

// OpenGL
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;
//

// std includes
#include <iostream>
#include <thread>
//

// Local Includes
#include "Engine.h"
#include "HelperTasks.h"
#include "Window.h"
#include "Sprite.h"
//

// STB image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	eng::init();
	
	auto w0 = eng::wnd::create(1024, 1024, "Window");
	auto w1 = eng::wnd::create(1024, 1024, "Window");

	w0->init();
	w1->init();

	auto spr = eng::spr::create(w0.get());
	spr->tex = eng::wnd::load_texture(w0.get(), "Resources/DK.png");

	while (!w0->KILL && !w1->KILL)
	{
		while (!w0->idle());
		while (!w1->idle());
	};

	w0->join();
	w1->join();

	w0.reset();
	w1.reset();

	delete spr;

	eng::close();

	return 0;
};
