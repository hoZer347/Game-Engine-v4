#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "loom.h"
#include "Data.h"
#include "Window.h"
using namespace loom;

#include <iostream>

int main()
{
	Window window;

	Shader shader { "shaders/default" };
	Textures textures { "Resources/stone.png" };
	Draw draw { 3 };
	Buffer<64, float> buffer { GL_ARRAY_BUFFER, GL_STATIC_DRAW };
	buffer =
	{
		0, 0, 0, 1,   1, 0, 0, 1,   1, 1, 1, 1,   0, 0, 0, 0,
		1, 0, 0, 1,   1, 0, 0, 1,   1, 1, 1, 1,   1, 0, 0, 0,
		1, 1, 0, 1,   1, 0, 0, 1,   1, 1, 1, 1,   1, 1, 0, 0,
		0, 1, 0, 1,   1, 0, 0, 1,   1, 1, 1, 1,   0, 1, 0, 0,
	};
	Mesh mesh { shader, textures, draw, buffer };

	std::cout << sizeof(Shader) << std::endl;
	std::cout << sizeof(Textures) << std::endl;
	std::cout << sizeof(Draw) << std::endl;
	std::cout << sizeof(Buffer<64, float>) << std::endl;
	std::cout << sizeof(Mesh) << std::endl;

	open(window, 640, 640, "Title");

	return 0;
};
