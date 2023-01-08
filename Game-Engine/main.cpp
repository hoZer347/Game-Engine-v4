#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "loom.h"
#include "Data.h"
#include "Window.h"
using namespace loom;

#include <iostream>
#include <thread>
#include <atomic>

static inline std::atomic<int> i1 = 0;
static inline int i2 = 0;

int main()
{
	Window window;

	Shader shader { "shaders/default" };
	Textures textures{ "Resources/stone.png" };
	Draw draw { 3 };
	Buffer<64, float> buffer { GL_ARRAY_BUFFER, GL_STATIC_DRAW };


	open(window, 640, 640, "Title");

	while (true);

	return 0;
};
