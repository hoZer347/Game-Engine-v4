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
	{
		squares.data.reserve(x_size * y_size * 8);

		for (float x = 0; x < x_size; x++)
		{
			for (float y = 0; y < y_size; y++)
				squares += vec4(x, y, 0, 1);
	};
	void Grid::load()
	{

	};
	void Grid::render()
	{
		
	};
	void Grid::unload()
	{

	};
};
