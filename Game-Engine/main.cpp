#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <glm/gtx/transform.hpp>

#include "Loom.h"
#include "Grid.h"
#include "Helper.h"
#include "Camera.h"
#include "Buffer.h"
#include "Geometry.h"
using namespace loom;

#include <thread>
#include <iostream>

int main()
{
	Loom::Init();

	DoubleBuffer buffer;

	std::thread thread1 = std::thread([&buffer]()
	{
		while (true)
		{
			buffer.access1([](std::vector<float>& v)
			{
				if (v.size())
					v.pop_back();
			});
		};
	});
	std::thread thread2 = std::thread([&buffer]()
	{
		while (true)
		{
			buffer.access2([](std::vector<float>& v)
			{
				v.push_back(1);
			});
		};
	});

	Loom::RunOnThisThread();

	Loom::Exit();

	return 0;
};
