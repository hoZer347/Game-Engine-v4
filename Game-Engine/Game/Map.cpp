#include "Map.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "../Camera.h"

#include <thread>

namespace loom
{
	static inline GLint _color, _mvp;

	Map::Map(const uint16_t& x, const uint16_t& y) :
		x(x), y(y),
		data(LAYERS::MAX, std::vector<std::vector<float>>(x, std::vector<float>(y, 0)))
	{ };
	
	GridOutline::GridOutline(Map& map)
	{
		reassign(map);
	};
	void GridOutline::reassign(Map& map)
	{
		const uint16_t x = map.x+1, y = map.y+1;

		vtxs.reserve(x * y);
		for (auto i = 0; i < x; i++)
			for (auto j = 0; j < y; j++)
				vtxs.emplace_back(vec4{ i, j, 0, 1 });

		inds.reserve((x-1) * (y-1) * 8);
		for (auto i = 0; i < x-1; i++)
			for (auto j = 0; j < y-1; j++)
				if (map[LAYERS::PASSABILITY][j][i] == 0)
				{
					inds.push_back((j + 0) + y * (i + 0));
					inds.push_back((j + 1) + y * (i + 0));
					inds.push_back((j + 1) + y * (i + 0));
					inds.push_back((j + 1) + y * (i + 1));
					inds.push_back((j + 1) + y * (i + 1));
					inds.push_back((j + 0) + y * (i + 1));
					inds.push_back((j + 0) + y * (i + 1));
					inds.push_back((j + 0) + y * (i + 0));
				};
	};
	void GridOutline::load()
	{
		_mvp = glGetUniformLocation(shader.id, "mvp");
		_color = glGetUniformLocation(shader.id, "color");
	};
	void GridOutline::render()
	{
		glUseProgram(shader.id);

		glEnableVertexAttribArray(VEC4_0_16);
		glUniformMatrix4fv(_mvp, 1, GL_FALSE, &Camera::mvp[0][0]);

		glBufferData(
			GL_ARRAY_BUFFER,
			vtxs.size() * sizeof(vec4),
			vtxs.data(),
			GL_DYNAMIC_DRAW);

		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			inds.size() * sizeof(uint32_t),
			inds.data(),
			GL_DYNAMIC_DRAW);

		glDrawElements(GL_LINES, (GLsizei)inds.size(), GL_UNSIGNED_INT, nullptr);

		glDisableVertexAttribArray(VEC4_0_16);

		glUseProgram(0);
	};


	Highlights::Highlights(Map& map)
	{
		reassign(map);
	};
	void Highlights::reassign(Map& map)
	{
		std::thread thread = std::thread([&, this]()
		{
			std::vector<vec4>*		vtxs = new std::vector<vec4>();
			std::vector<uint32_t>*	inds = new std::vector<uint32_t>();



			delete this->vtxs.exchange(vtxs);
			delete this->inds.exchange(inds);
		});
		thread.detach();
	};
	void Highlights::render()
	{

	};
};
