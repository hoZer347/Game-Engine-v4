#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <glm/gtx/transform.hpp>

#include "Loom.h"
#include "Grid.h"
#include "Camera.h"
#include "Geometry.h"
using namespace loom;

#include <thread>
#include <iostream>

int main()
{
	Loom::Init();

	Camera camera;

	Squares squares;
	squares += vec4(-.5, -.5, 0, 1);

	Helper helper0;
	Helper helper1;
	Helper helper2;
	Helper helper3;
	Helper helper4;
	Helper helper5;
	Helper helper6;
	Helper helper7;

	Loom::RunOnThisThread();

	Loom::Exit();

	return 0;
};
