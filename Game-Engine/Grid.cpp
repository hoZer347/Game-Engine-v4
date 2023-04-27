#include "Grid.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/intersect.hpp>
#include "Enums.h"
#include "Camera.h"

#include <thread>

namespace loom
{
	static inline Shader shader{ "shaders/Grid" };

	Grid::Grid(uint32_t x_size, uint32_t y_size)
	: x_size(x_size), y_size(y_size)
	{
		for (size_t i = 0; i < (size_t)x_size; i++)
		{
			cells.emplace_back();
			for (size_t j = 0; j < (size_t)y_size; j++)
				cells.back().emplace_back(vec2(i, j));
		};
	};
	Grid::~Grid()
	{

	};
	void Grid::ConstructCells(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, bool connected)
	{
		if (connected)
		{
			for (uint32_t i = x0; i < x1; i++)
				for (uint32_t j = y0; j < y1; j++)
				{
					cells[i][j].v0 = (uint32_t)inds.size() + 0;
					cells[i][j].v1 = (uint32_t)inds.size() + 2;
					cells[i][j].v2 = (uint32_t)inds.size() + 4;
					cells[i][j].v3 = (uint32_t)inds.size() + 6;

					inds.push_back((uint32_t)vtxs.size() + 0);
					inds.push_back((uint32_t)vtxs.size() + 1);
					inds.push_back((uint32_t)vtxs.size() + 1);
					inds.push_back((uint32_t)vtxs.size() + 2);
					inds.push_back((uint32_t)vtxs.size() + 2);
					inds.push_back((uint32_t)vtxs.size() + 3);
					inds.push_back((uint32_t)vtxs.size() + 3);
					inds.push_back((uint32_t)vtxs.size() + 0);

					vtxs.emplace_back(i,     j,     i*j/10.f, 1);
					vtxs.emplace_back(i + 1, j,     i*j/10.f, 1);
					vtxs.emplace_back(i + 1, j + 1, i*j/10.f, 1);
					vtxs.emplace_back(i,     j + 1, i*j/10.f, 1);
				};
		}
		else
		{
			// TODO:
		};
	};
	void Grid::load()
	{
		_mvp = glGetUniformLocation(shader.id, "mvp");
		_trns = glGetUniformLocation(shader.id, "trns");
	};
	void Grid::render()
	{
		glUseProgram(shader.id);
		glEnableVertexAttribArray(VEC4_0_16);
		glUniformMatrix4fv(_mvp, 1, GL_FALSE, &Camera::mvp[0][0]);


		// Drawing the grid lines
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * vtxs.size(), vtxs.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * inds.size(), inds.data(), GL_STATIC_DRAW);
		glDrawElements(GL_LINES, (GLsizei)inds.size(), GL_UNSIGNED_INT, 0);
		//



		// Drawing the hovered Cell
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * Hovered::inds.size(), Hovered::inds.data(), GL_STATIC_DRAW);
		glDrawElements(GL_QUADS, (GLsizei)Hovered::inds.size(), GL_UNSIGNED_INT, 0);
		//


		glDisableVertexAttribArray(VEC4_0_16);
		glUseProgram(0);
	};
};
