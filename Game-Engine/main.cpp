#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <glm/gtx/transform.hpp>

#include "Loom.h"
#include "Helper.h"
#include "Camera.h"
#include "Buffer.h"
#include "Geometry.h"
using namespace loom;

#include <thread>
#include <chrono>
#include <iostream>

struct Test
{
	void render()
	{
		for (auto& i : data)
			std::cout << i << ", ";
		std::cout << ";" << std::endl;
	};

	void load()   { std::cout << "Load" << std::endl; };
	void unload() { std::cout << "Unload" << std::endl; };

	std::vector<float> data;
};

typedef std::chrono::high_resolution_clock Clock;
std::mutex mut;

int main()
{
	Loom::Init();
	 
	Loom::RunOnThisThread();

	Loom::Exit();

	return 0;
};
