#include "Grid.h"


#include "GLEW/glew.h"
#include "GLFW/glfw3.h"


#include <glm/gtx/transform.hpp>
#include <glm/gtx/intersect.hpp>


#include "../Utils.h"
#include "../Enums.h"
#include "../Camera.h"
#include "Unit.h"

#include <thread>


namespace loom
{
	static inline GLint _color, _trns, _mvp;
	static inline Shader shader{ "Game/Grid" };


	Grid::Grid(size_t x_size, size_t y_size)
	: x_size(x_size), y_size(y_size)
	{
		// Creating Cells
		for (size_t i = 0; i < (size_t)x_size; i++)
		{
			cells.emplace_back();
			for (size_t j = 0; j < (size_t)y_size; j++)
				cells.back().emplace_back(new Cell{ vec2(i, j) });
		};


		// Assigning Cell Associations
		for (uint32_t i = 1; i < x_size - 1; i++)
			for (uint32_t j = 1; j < y_size - 1; j++)
			{
				(*this)[i][j]->U = (*this)[i+1][j];
				(*this)[i][j]->D = (*this)[i-1][j];
				(*this)[i][j]->L = (*this)[i][j-1];
				(*this)[i][j]->R = (*this)[i][j+1];
			};


		// Adding Vertex Data
		for (size_t i = 0; i < x_size; i++)
			for (size_t j = 0; j < y_size; j++)
			{
				cells[i][j]->v0 = (uint32_t)inds.size() + 0;
				cells[i][j]->v1 = (uint32_t)inds.size() + 2;
				cells[i][j]->v2 = (uint32_t)inds.size() + 4;
				cells[i][j]->v3 = (uint32_t)inds.size() + 6;

				inds.push_back((uint32_t)vtxs.size() + 0);
				inds.push_back((uint32_t)vtxs.size() + 1);
				inds.push_back((uint32_t)vtxs.size() + 1);
				inds.push_back((uint32_t)vtxs.size() + 2);
				inds.push_back((uint32_t)vtxs.size() + 2);
				inds.push_back((uint32_t)vtxs.size() + 3);
				inds.push_back((uint32_t)vtxs.size() + 3);
				inds.push_back((uint32_t)vtxs.size() + 0);

				vtxs.emplace_back(i, j, 0, 1);
				vtxs.emplace_back(i + 1, j, 0, 1);
				vtxs.emplace_back(i + 1, j + 1, 0, 1);
				vtxs.emplace_back(i, j + 1, 0, 1);
			};
	};
	Grid::~Grid()
	{

	};
	void Grid::addUnit(Unit* unit, uint32_t x, uint32_t y)
	{
		Cell* cell = cells[x][y];
		cell->unit = unit;
		unit->trns = translate(vec3(vtxs[inds[cell->v0]]) + vec3(0, .5, 0));
	};
	void Grid::load()
	{
		_mvp = glGetUniformLocation(shader.id, "mvp");
		_trns = glGetUniformLocation(shader.id, "trns");
		_color = glGetUniformLocation(shader.id, "color");
	};
	void Grid::update()
	{
		static Utils::Timer timer;

		if (timer.GetDiff_mls() < 1000.f / 60)
			return;
		timer.push(std::chrono::milliseconds(1000 / 60));

		Loom::Exec([&]() {
			vec2 b;
			float d;

			for (auto& _cells : cells)
				for (auto& cell : _cells)
					if (cell->v0 == -1) continue;
					else if (intersectRayTriangle(vec3(Camera::mpos), vec3(Camera::mdir),
						vec3(vtxs[inds[cell->v0]]),
						vec3(vtxs[inds[cell->v1]]),
						vec3(vtxs[inds[cell->v2]]),
						b, d) ||
						intersectRayTriangle(vec3(Camera::mpos), vec3(Camera::mdir),
							vec3(vtxs[inds[cell->v0]]),
							vec3(vtxs[inds[cell->v3]]),
							vec3(vtxs[inds[cell->v2]]),
							b, d))
					{
						if (cell == Grid::Hovered::cell)
							return;
						Grid::Hovered::cell = cell;
						Grid::Hovered::inds =
						{
							inds[cell->v0],
							inds[cell->v1],
							inds[cell->v2],
							inds[cell->v3],
						};
					};
		});
	};
	void Grid::render()
	{
		glUseProgram(shader.id);
		glEnableVertexAttribArray(VEC4_0_16);
		glUniformMatrix4fv(_mvp, 1, GL_FALSE, &Camera::mvp[0][0]);


		// Drawing the constructed grid lines
		glUniform4fv(_color, 1, &vec4(.7, .7, .7, 1)[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * vtxs.size(), vtxs.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * inds.size(), inds.data(), GL_STATIC_DRAW);
		glDrawElements(GL_LINES, (GLsizei)inds.size(), GL_UNSIGNED_INT, 0);
		//


		// Drawing flooded squares
		std::vector<uint32_t>* vec;
		if (vec = reds.load())
		{
			glUniform4fv(_color, 1, &vec4(.5, 0, 0, 1)[0]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * vec->size(), vec->data(), GL_STATIC_DRAW);
			glDrawElements(GL_QUADS, (GLsizei)vec->size(), GL_UNSIGNED_INT, 0);
		};
		if (vec = blues.load())
		{
			glUniform4fv(_color, 1, &vec4(0, 0, .5, 1)[0]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * vec->size(), vec->data(), GL_STATIC_DRAW);
			glDrawElements(GL_QUADS, (GLsizei)vec->size(), GL_UNSIGNED_INT, 0);
		};


		// Drawing the hovered Cell
		glUniform4fv(_color, 1, &vec4(1, 0, 0, 1)[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * Hovered::inds.size(), Hovered::inds.data(), GL_STATIC_DRAW);
		glDrawElements(GL_QUADS, (GLsizei)Hovered::inds.size(), GL_UNSIGNED_INT, 0);
		//


		glDisableVertexAttribArray(VEC4_0_16);
		glUseProgram(0);
	};
};
