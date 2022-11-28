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
#define LOG_DATA_CREATES
#define LOG_THREAD_ASSIGNS

#include "Task.h"
#include "Input.h"
#include "Engine.h"
#include "Renderables.h"
#include "Data.h"
using namespace eng;
//

// STB Image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//

int main()
{
	start();

	Mesh::test();

	auto w0 = Data<Window>::create();

	Task t0([]()
		{
			std::cout << "Test" << std::endl;

			return false;
		});

	t0();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	w0->assign([]()
		{
			std::cout << "Test4" << std::endl;
			
			return true;
		});
	std::cout << "Test5" << std::endl;
	Data<int>::create(50);

	while (!w0->get_kill());

	close();

	return 0;
};
