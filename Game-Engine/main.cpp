#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "loom.h"
using namespace loom;

#include <iostream>

int main()
{
	init();

	Shader		shader = 0;
	Texture		texture = 0;
	DrawMode	draw_mode = GL_QUADS;
	Mesh		mesh =
	{
		{
			{ vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(0, 0, 0, 1), },
			{ vec4(1, 0, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(1, 0, 0, 1), },
			{ vec4(1, 1, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(1, 1, 0, 1), },
			{ vec4(0, 1, 0, 1), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(0, 1, 0, 1), },
		}, { 0, 1, 2, 3 },
	};

	Thread window;

	open_window(window);

	load(shader, window, "Test0", { "shaders/default" });
	load(texture, window, shader, "Test1", "Resources/DK.png", GL_RGBA);
	load(mesh, window, shader, texture, draw_mode);

	assign(window, []()
	{
		std::cout << "Test" << std::endl;
	});

	run({ &window });

	exit();
};
