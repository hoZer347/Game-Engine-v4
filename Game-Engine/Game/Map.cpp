#include "Map.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "../Camera.h"

#include <thread>

namespace loom
{
	static inline GLint _color, _mvp;

	Map::Map(const uint8& w, const uint8& h) :
		w(w), h(h)
	{
		vtxs.reserve((w + 1) * (h + 1));
		cells.reserve(w * h);
		
		for (auto j = 0; j < (h + 1); j++)
			for (auto i = 0; i < (w + 1); i++)
				vtxs.emplace_back(vec4{ i, j, 0, 1});

		for (auto i = 0; i < w; i++)
			for (auto j = 0; j < h; j++)
				cells.emplace_back(
					Cell{ { i, j }, 0, 0,
					(uint32)(i + 0) + (w + 1) * (j + 0),
					(uint32)(i + 1) + (w + 1) * (j + 0),
					(uint32)(i + 1) + (w + 1) * (j + 1),
					(uint32)(i + 0) + (w + 1) * (j + 1), });

		for (auto i = 0; i < w; i++)
			for (auto j = 0; j < h; j++)
			{
				if (i)			(*this)[i][j].L = &(*this)[i-1][j];
				if (i < w-1)	(*this)[i][j].R = &(*this)[i+1][j];
				if (j)			(*this)[i][j].U = &(*this)[i][j+1];
				if (j < h-1)	(*this)[i][j].D = &(*this)[i][j-1];
			};
	};
	

	GridOutline::GridOutline(Map& map) :
		map(map)
	{
		reassign(map);
	};
	void GridOutline::reassign(Map& map)
	{
		inds.clear();
		inds.reserve(map.w * map.h * 8);
		for (auto& cell : map.cells)
		{
			inds.emplace_back(cell.v0);
			inds.emplace_back(cell.v1);
			inds.emplace_back(cell.v1);
			inds.emplace_back(cell.v2);
			inds.emplace_back(cell.v2);
			inds.emplace_back(cell.v3);
			inds.emplace_back(cell.v3);
			inds.emplace_back(cell.v0);
		};
	};
	void GridOutline::load()
	{
		if (!_mvp)
			_mvp = glGetUniformLocation(map.shader.id, "mvp");
		if (!_color)
			_color = glGetUniformLocation(map.shader.id, "color");
	};
	void GridOutline::render()
	{
		glUseProgram(map.shader.id);
		glEnableVertexAttribArray(VEC4_0_16);
		glUniformMatrix4fv(_mvp, 1, GL_FALSE, &Camera::mvp[0][0]);

		glBufferData(
			GL_ARRAY_BUFFER,
			map.vtxs.size() * sizeof(vec4),
			map.vtxs.data(),
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


	Highlights::Highlights(Map& map) :
		map(map)
	{
		reassign();
	}
	void Highlights::reassign(vec4&& new_color)
	{
		std::thread thread = std::thread([this, new_color]()
		{
			std::shared_ptr<std::vector<uint32>> _inds = std::make_shared<std::vector<uint32>>();
			std::shared_ptr<std::vector<uint32>> _outs = std::make_shared<std::vector<uint32>>();

			for (auto i = 0; i < map.w; i++)
				for (auto j = 0; j < map.h; j++)
				{
					Cell& cell = map[i][j];

					if (!cell.highlight)
						continue;

					if (cell.U && cell.U->highlight != cell.highlight || !cell.U)
					{
						_outs->push_back(cell.v2);
						_outs->push_back(cell.v3);
					};
					if (cell.D && cell.D->highlight != cell.highlight || !cell.D)
					{
						_outs->push_back(cell.v0);
						_outs->push_back(cell.v1);
					};
					if (cell.L && cell.L->highlight != cell.highlight || !cell.L)
					{
						_outs->push_back(cell.v3);
						_outs->push_back(cell.v0);
					};
					if (cell.R && cell.R->highlight != cell.highlight || !cell.R)
					{
						_outs->push_back(cell.v1);
						_outs->push_back(cell.v2);
					};

					_inds->push_back(map[i][j].v0);
					_inds->push_back(map[i][j].v1);
					_inds->push_back(map[i][j].v2);
					_inds->push_back(map[i][j].v3);
				};

			inds.exchange(_inds);
			outs.exchange(_outs);
		});
		thread.detach();
	};
	void Highlights::load()
	{
		if (!_mvp)
			_mvp = glGetUniformLocation(map.shader.id, "mvp");
		if (!_color)
			_color = glGetUniformLocation(map.shader.id, "color");
	};
	void Highlights::render()
	{
		std::shared_ptr<std::vector<uint32>> _inds = inds.load();
		std::shared_ptr<std::vector<uint32>> _outs = outs.load();

		if (_inds && _outs)
		{
			glUseProgram(map.shader.id);
			glEnableVertexAttribArray(VEC4_0_16);
			glUniformMatrix4fv(_mvp, 1, GL_FALSE, &Camera::mvp[0][0]);

			glLineWidth(20);

			glUniform4fv(_color, 1, &vec4(0, 0, .5, .5)[0]);
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				_inds->size() * sizeof(uint32),
				_inds->data(),
				GL_DYNAMIC_DRAW);
			glDrawElements(GL_QUADS, (GLsizei)_inds->size(), GL_UNSIGNED_INT, nullptr);
			
			glUniform4fv(_color, 1, &vec4(0, 0, 0, .5)[0]);
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				_outs->size() * sizeof(uint32),
				_outs->data(),
				GL_DYNAMIC_DRAW);
			glDrawElements(GL_LINES, (GLsizei)_outs->size(), GL_UNSIGNED_INT, nullptr);

			glLineWidth(1);

			glDisableVertexAttribArray(VEC4_0_16);
			glUseProgram(0);
		};
	};
};
