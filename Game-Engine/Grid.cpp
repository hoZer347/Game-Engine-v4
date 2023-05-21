#include "Grid.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/intersect.hpp>
#include "Utils.h"
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

					vtxs.emplace_back(i,     j,     0, 1);
					vtxs.emplace_back(i + 1, j,     0, 1);
					vtxs.emplace_back(i + 1, j + 1, 0, 1);
					vtxs.emplace_back(i,     j + 1, 0, 1);
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
	}
	void Grid::update()
	{
		static Utils::Timer timer;

		if (timer.GetDiff_mls() < 1000.f / 60)
			return;
		timer.push(std::chrono::milliseconds(1000 / 60));

		vec2 b;
		float d;

		for (auto& _cells : cells)
			for (auto& cell : _cells)
				if (cell.v0 == -1) { std::cout << "Test: " << cell.pos.x << ", " << cell.pos.y << std::endl; continue; }
				else if (intersectRayTriangle(vec3(Camera::mpos), vec3(Camera::mdir),
					vec3(vtxs[inds[cell.v0]]),
					vec3(vtxs[inds[cell.v1]]),
					vec3(vtxs[inds[cell.v2]]),
					b, d) ||
					intersectRayTriangle(vec3(Camera::mpos), vec3(Camera::mdir),
						vec3(vtxs[inds[cell.v0]]),
						vec3(vtxs[inds[cell.v3]]),
						vec3(vtxs[inds[cell.v2]]),
						b, d))
				{
					if (&cell == Grid::Hovered::cell)
						return;
					if (Grid::Hovered::cell)
						Grid::Hovered::cell->onUnhover();
					Grid::Hovered::cell = &cell;
					Grid::Hovered::cell->onHover();
					Grid::Hovered::inds =
					{
						inds[cell.v0],
						inds[cell.v1],
						inds[cell.v2],
						inds[cell.v3],
					};
				};
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
