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
#include "Renderables.h"
#include "Data.h"
using namespace eng;
//

// STB Image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//

static inline std::atomic<int> _i = 0;

int main()
{
	auto w0 = Data<Window>::create();
	auto t0 = Data<Thread>::create();
	auto t1 = Data<Thread>::create();
	auto t2 = Data<Thread>::create();
	auto m0 = Data<Mesh>::create();

	w0->assign([]()
		{
			std::cout << _i++ << std::endl;
		});
	t0->assign([]()
		{
			std::cout << _i++ << std::endl;
		});
	t1->assign([]()
		{
			std::cout << _i++ << std::endl;
		});
	t2->assign([]()
		{
			std::cout << _i++ << std::endl;
		});

	Buffer<Mesh::r_Obj>::create();
	Buffer<Mesh::r_Obj>::access([](auto vec)
		{
			vec[0]->vtxs =
			{
				{
					{ vec4(0, 0, 0, 1), vec4(1), vec4(1), vec4(0, 0, 0, 0), },
					{ vec4(1, 0, 0, 1), vec4(1), vec4(1), vec4(1, 0, 0, 0), },
					{ vec4(1, 1, 0, 1), vec4(1), vec4(1), vec4(1, 1, 0, 0), },
					{ vec4(0, 1, 0, 1), vec4(1), vec4(1), vec4(0, 1, 0, 0), },
				}
			};
			vec[0]->inds = { 0, 1, 2, 3 };
		});

	Sync<0, 4>::create({ w0, t0, t1, t2 });

	m0->load(w0);
	m0->load_shader({ "Sprite" });
	m0->load_textures({ { "Resources/DK.png", GL_RGBA } });

	std::cout << "Test1" << std::endl;
	w0.reset();
	std::cout << "Test2" << std::endl;
	t0.reset();
	std::cout << "Test3" << std::endl;
	t1.reset();
	std::cout << "Test4" << std::endl;
	t2.reset();
	std::cout << "Test5" << std::endl;

	while (w0 && t0 && t1 && t2);

	//while (!w0->get_kill());

	return 0;
};
