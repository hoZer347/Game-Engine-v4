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

	Input::KeyHold([]() { std::cout << "Test" << std::endl; }, GLFW_KEY_W, GLFW_PRESS, 0);
	Loom::RunOnThisThread();

	Loom::Exit();

	return 0;
};
