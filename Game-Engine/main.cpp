#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <glm/gtx/transform.hpp>

#include "Loom.h"
#include "Grid.h"
#include "Input.h"
#include "Helper.h"
#include "Camera.h"
#include "Buffer.h"
#include "Geometry.h"
using namespace loom;

#include <thread>
#include <chrono>
#include <iostream>
#include <barrier>

int main()
{
	Loom::Init();
	
	Camera camera;
	Grid grid{ 5, 3 };
	
	Input::MouseButtonPress([]()
	{	std::cout << "Test0" << std::endl;
		Input::next();
		Input::MouseButtonPress([]()
		{	std::cout << "Test1" << std::endl;
			Input::next();
			Input::MouseButtonPress([]()
			{	std::cout << "Test2" << std::endl;
				Input::prev();
				Input::prev();
			}, GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0);
		}, GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0);
	}, GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0);

	Loom::RunOnThisThread();

	Loom::Exit();

	return 0;
};
