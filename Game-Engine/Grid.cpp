#include "Grid.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <glm/gtx/transform.hpp>

#include "Data.h"
#include "Enums.h"
#include "Camera.h"

#include <thread>

namespace loom
{
	Grid::Grid(uint16_t x_size, uint16_t y_size)
	: x_size(x_size += 1), y_size(y_size += 1)
	{
		float s = std::max(x_size, y_size);
		std::thread _vtxs = std::thread([s, x_size, y_size, this]()
		{
			vtxs = std::make_unique<Cell[]>(x_size * y_size);
			for (auto x = 0; x < x_size; x++)
				for (auto y = 0; y < y_size; y++)
					vtxs[x * y_size + y] = { vec4{(float)x / s, (float)y / s, 0, 1 } };
		});

		std::thread _inds = std::thread([s, x_size, y_size, this]()
		{
			inds.reserve(4 * x_size * y_size);
			for (auto x = 0; x < x_size-1; x++)
				for (auto y = 0; y < y_size-1; y++)
				{
					inds.emplace_back(x * y_size + y);
					inds.emplace_back((x + 1) * y_size + y);
					inds.emplace_back((x + 1) * y_size + y);
					inds.emplace_back((x + 1) * y_size + (y + 1));
					inds.emplace_back((x + 1) * y_size + (y + 1));
					inds.emplace_back(x * y_size + (y + 1));
					inds.emplace_back(x * y_size + (y + 1));
					inds.emplace_back(x * y_size + y);
				};
		});

		trns = translate(vec3(-.5f, -.5f, 0));

		if (_vtxs.joinable())
			_vtxs.join();
		if (_inds.joinable())
			_inds.join();
	};
	Grid::~Grid()
	{

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
		glUniformMatrix4fv(_mvp, 1, GL_FALSE, &trns[0][0]); glGetError();
		glUniformMatrix4fv(_trns, 1, GL_FALSE, &Camera::mvp[0][0]); glGetError();
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * x_size * y_size, vtxs.get(), GL_STREAM_DRAW); glGetError();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * inds.size(), inds.data(), GL_STREAM_DRAW); glGetError();
		glDrawElements(GL_LINES, (GLsizei)inds.size(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(VEC4_0_16);
		glUseProgram(0);
	};
	void Grid::unload()
	{
		// TODO:
	};
};
