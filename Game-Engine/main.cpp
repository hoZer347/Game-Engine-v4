#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <glm/gtx/transform.hpp>
using namespace glm;

#include "Loom.h"
#include "Grid.h"
#include "Text.h"
#include "Utils.h"
#include "Input.h"
#include "Sprite.h"
#include "Camera.h"
#include "Commands.h"
#include "Geometry.h"
using namespace loom;

#include <thread>
#include <chrono>
#include <iostream>

// TODO: Text

int main()
{
	Loom::Init();

	Grid grid { 10, 10 };
	grid.ConstructCells(0, 0, 10, 10);

	Commands::bindCameraToGrid(grid);

	Texture texture{ "Resources/Anna.png", GL_RGBA };
	Sprite sprite{ texture, { 12 * 32, 12 * 32 }, { 32, 32 }, { 32, 0 }, 1 };

	Loom::Run();

	Loom::Exit();

	return 0;
};
