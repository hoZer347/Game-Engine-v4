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
#define ENG_DEBUG
#include "Engine.h"
using namespace eng;
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
	eng::Sprite::init(w0.get());
	
	Object<Sprite>::create(2);

	Object<Sprite>::access([&w0](Sprite& spr) {
		wnd::load_texture(spr.tex, w0.get(), "Resources/stone.png", GL_RGB);
		}, 0);

	Object<Sprite>::access([&w0](Sprite& spr) {
		wnd::load_texture(spr.tex, w0.get(), "Resources/DK.png", GL_RGBA);
		}, 1);

	while (!w0->KILL || !w1->KILL)
	{
		while (!w0->idle());
		while (!w1->idle());
	};

	eng::close();

	return 0;
};
