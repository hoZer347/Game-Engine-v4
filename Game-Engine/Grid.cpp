#include "Grid.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Loom.h"

#include "Texture.h"
#include "Shader.h"

namespace loom
{
	void Cell::load()
	{

	};
	void Cell::unload()
	{

	};

	

	Grid::Grid(uint32_t x_size, uint32_t y_size)
	: x_size(x_size), y_size(y_size)
	{ };
	void Grid::render()
	{ };
};
