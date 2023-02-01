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
#include <memory>
#include <barrier>
#include <iostream>
#include <type_traits>

int main()
{
	Loom::Init();

	auto camera = Utils::Construct<Camera>();
	std::vector<std::unique_ptr<Grid>> grids;
	grids.push_back(Utils::Construct<Grid>(32, 32));


	Loom::RunOnThisThread();

	Loom::Exit();

	return 0;
};
