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
#include <thread>
//

// Local Includes
#include "Engine.h"
#include "Timing.h"
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
	eng::SpriteData::init(w0.get());

	
	eng::SpriteData::create(2);

	eng::SpriteData::lock();
	eng::SpriteData::data()[0].tex = eng::wnd::load_texture(w0.get(), "Resources/stone.png", GL_RGB);
	eng::SpriteData::unlock();

	eng::SpriteData::lock();
	eng::SpriteData::data()[1].tex = eng::wnd::load_texture(w0.get(), "Resources/DK.png", GL_RGBA);
	eng::SpriteData::unlock();

	while (!w0->KILL || !w1->KILL)
	{
		while (!w0->idle());
		while (!w1->idle());
	};

	//delete spr1;
	//delete spr2;

	eng::close();

	return 0;
};
