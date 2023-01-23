#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <glm/gtx/transform.hpp>

#include "Loom.h"
#include "Grid.h"
#include "Utils.h"
#include "Input.h"
#include "Helper.h"
#include "Camera.h"
#include "Buffer.h"
#include "Geometry.h"
using namespace loom;

#include <thread>
#include <chrono>
#include <barrier>
#include <iostream>

// TODO: Might be possible to take #include "Manage.h" out of "Data.h"

int main()
{
	Loom::Init();
	
	Camera camera;

	std::atomic<bool> KILL = false;
	std::unique_ptr<Grid> grid;
	Loom::Construct(grid, 100, 100);

	vec4 _finish = vec4(.5, 0, 0, 1);
	vec4 buh = vec4(1) *= 5;

	auto t = transform::approach(camera.ctr, _finish, 2, 1);

	Helper helper1{ [&_finish]()
	{
		_finish = rotate(0.0004f, vec3(0, 1, 0)) * _finish;
	} };

	Input::KeyHold([]() { std::cout << "Test" << std::endl; }, GLFW_KEY_W, GLFW_PRESS, 0);
	Loom::RunOnThisThread();

	Loom::Exit();

	return 0;
};
