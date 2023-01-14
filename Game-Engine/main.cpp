#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <glm/gtx/transform.hpp>

#include "Loom.h"
#include "Grid.h"
#include "Geometry.h"
using namespace loom;

#include <thread>
#include <iostream>

int main()
{
	Loom::Init();

	Grid grid{ 100, 100 };
	grid.squares.trns *= scale(vec3(1.5, 1.5, 1.5));
	grid.squares.trns *= translate(vec3(-.5, -.5, 0));

	Loom::RunOnThisThread();

	Loom::Exit();

	return 0;
};
