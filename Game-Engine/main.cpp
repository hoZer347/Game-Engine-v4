#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "loom.h"
using namespace loom;

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include <iostream>

int main()
{
	init();

	Shader		shader = 0;
	Texture		texture = 0;
	DrawMode	draw_mode = GL_QUADS;

	std::cout << sizeof(Thread) << std::endl;

	Thread window;

	open_window(window);

	load(shader, window, "Test0", { "shaders/default" });
	load(texture, window, shader, "Test1", "Resources/DK.png", GL_RGBA);

	double max = 100;
	std::vector<Mesh> meshes;
	meshes.reserve((size_t)(max * max));
	for (double x = 0; x < max; x++)
		for (double y = 0; y < max; y++)
		{
			double x0 = x / max;
			double y0 = y / max;

			double x1 = (x+1) / max;
			double y1 = (y+1) / max;

			meshes.emplace_back(Mesh(
			{
				{
					{ vec4(x0, y0, 0.f, 1.f), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(0, 0, 0, 1), },
					{ vec4(x1, y0, 0.f, 1.f), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(1, 0, 0, 1), },
					{ vec4(x1, y1, 0.f, 1.f), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(1, 1, 0, 1), },
					{ vec4(x0, y1, 0.f, 1.f), vec4(1, 1, 1, 1), vec4(1, 1, 1, 1), vec4(0, 1, 0, 1), },
				}, { 0, 1, 2, 3 },
			}));
			//meshes.back().trns = translate(mat4(1), vec3(x0, y0, 1));
			load(meshes.back(), window, shader, texture, draw_mode);
		};

	run({ &window });

	exit();
};
