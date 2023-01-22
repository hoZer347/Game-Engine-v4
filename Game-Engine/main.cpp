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
#include <iostream>
#include <barrier>

struct Buh : public Manage<Object>
{

};

int main()
{
	Loom::Init();
	
	Camera camera;
	Grid grid{ 5, 3 };

	vec4 _finish = vec4(1, 1, 0, 0);
	vec4 buh = vec4(1) *= 5;

	Approach app (camera.ctr, _finish, 0.1, 1);

	SyncHelper helper0{ [&app]()
	{
		app();
	} };

	SyncHelper helper1{ [&_finish]()
	{
		//_finish = rotate(0.0004f, vec3(0, 1, 0)) * _finish;
	} };

	Input::KeyHold([]() { std::cout << "Test" << std::endl; }, GLFW_KEY_W, GLFW_PRESS, 0);
	Loom::RunOnThisThread();

	Loom::Exit();

	return 0;
};
