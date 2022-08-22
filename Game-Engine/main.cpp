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

			input::next();

			input::bind([]()
				{
					std::cout << "Test2" << std::endl;

					input::prev();

					return false;
				}, GLFW_MOUSE_BUTTON_1, GLFW_RELEASE, 0);

			return false;
		}, GLFW_MOUSE_BUTTON_1, GLFW_PRESS, 0);

	input::bind([w0]()
		{
			GLFWwindow* window = glfwGetCurrentContext();

			Camera& cam = wnd::get_cam(w0);

			if (glfwGetKey(window, GLFW_KEY_W)) cam.trns					*= translate(vec3(0, 0,  0.001));
			if (glfwGetKey(window, GLFW_KEY_S)) cam.trns					*= translate(vec3(0, 0, -0.001));
			if (glfwGetKey(window, GLFW_KEY_A)) cam.trns					*= translate(vec3( 0.001, 0, 0));
			if (glfwGetKey(window, GLFW_KEY_D)) cam.trns					*= translate(vec3(-0.001, 0, 0));
			if (glfwGetKey(window, GLFW_KEY_SPACE)) cam.trns				*= translate(vec3(0, -0.001, 0));
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) cam.trns			*= translate(vec3(0,  0.001, 0));

		}, 0);

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	join();

	close();

	return 0;
};
