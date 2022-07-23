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
using namespace eng;
//

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Material mat;
	std::vector<Mesh> m = { Mesh(
		{
			Vtx({vec4(0, 0, 0, 1), vec4(0, 0, 0, 0), vec4(0, 0, 0, 0), vec4(0, 0, 0, 0)}),
			Vtx({vec4(1, 0, 0, 1), vec4(1, 0, 0, 0), vec4(0, 0, 0, 0), vec4(0, 0, 0, 0)}),
			Vtx({vec4(1, 1, 0, 1), vec4(1, 1, 0, 0), vec4(0, 0, 0, 0), vec4(0, 0, 0, 0)}),
			Vtx({vec4(0, 0, 0, 1), vec4(0, 0, 0, 0), vec4(0, 0, 0, 0), vec4(0, 0, 0, 0)}),
			Vtx({vec4(0, 1, 0, 1), vec4(0, 1, 0, 0), vec4(0, 0, 0, 0), vec4(0, 0, 0, 0)}),
			Vtx({vec4(1, 1, 0, 1), vec4(1, 1, 0, 0), vec4(0, 0, 0, 0), vec4(0, 0, 0, 0)}),
		}, { 0, 1, 2, 3, 4, 5 }
	) };
	mat.draw_mode = GL_TRIANGLES;

	Thread<0>::init();
	Thread<1>::init();
	Thread<2>::init();

	Renderer::open_window<0>();
	Text::init<0>();
	Input::init<2>();
	Renderer::render_this<0>(mat, m);
	//Renderer::load_shader<0>(mat, "Sprite");
	//Renderer::load_texture<0>(mat, "Resources/DK.png", GL_RGBA);
	Renderer::load_shader<0>(mat, "Text");
	Text::load_font<0>(mat, "Resources/alagard.ttf");

	Thread<0>::join();

	return 0;
};
