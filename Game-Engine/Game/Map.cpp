#include "Map.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtx/intersect.hpp"

#include "../Utils.h"
#include "../Camera.h"
#include "../Geometry.h"

#include "Cell.h"

#include <thread>

namespace loom
{
	static inline GLint _color, _mvp;


	Map::Map(const uint8& w, const uint8& h) :
		w(w), h(h)
	{
		vtxs.reserve((w + 1) * (h + 1));
		cells.reserve(w * h);
		
		hovered.color = vec4(1, 0, 0, .5);


		for (auto j = 0; j < (h + 1); j++)
			for (auto i = 0; i < (w + 1); i++)
				vtxs.emplace_back(vec4{ i, j, 0, 1});


		for (auto i = 0; i < w; i++)
			for (auto j = 0; j < h; j++)
				cells.emplace_back(
					Cell{ { i, j }, 0,
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
	void Map::reset_highlights()
	{
		for (auto& cell : cells)
			cell.highlight = 0;
	};
	void Map::update()
	{
		static Utils::Timer timer;

		if (timer.GetDiff_mls() < 1000.f / 60)
			return;
		timer.push(std::chrono::milliseconds(1000 / 60));

		Loom::Exec([&]() {
			vec2 b;
			float d;

			for (auto& cell : cells)
				if (&cell && cell.v0 == -1) continue;
				else if (intersectRayTriangle(vec3(Camera::mpos), vec3(Camera::mdir),
					vec3(vtxs[cell.v0]),
					vec3(vtxs[cell.v1]),
					vec3(vtxs[cell.v2]),
					b, d) ||
					intersectRayTriangle(vec3(Camera::mpos), vec3(Camera::mdir),
					vec3(vtxs[cell.v0]),
					vec3(vtxs[cell.v3]),
					vec3(vtxs[cell.v2]),
					b, d))
				{
					hovered.vtxs[0] = vtxs[cell.v0];
					hovered.vtxs[1] = vtxs[cell.v1];
					hovered.vtxs[2] = vtxs[cell.v2];
					hovered.vtxs[3] = vtxs[cell.v3];

					break;
				};
		});
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
		_mvp = glGetUniformLocation(map.shader.id, "mvp");
		_color = glGetUniformLocation(map.shader.id, "color");
	};
	void GridOutline::render()
	{
		if (!show)
			return;

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
	};
	void Highlights::reassign(vec4&& new_color)
	{
		std::thread thread = std::thread([this, new_color]()
		{
			std::shared_ptr<std::vector<vec4>>		_vtxs = std::make_shared<std::vector<vec4>>();
			std::shared_ptr<std::vector<uint32>>	_inds = std::make_shared<std::vector<uint32>>();
			std::shared_ptr<std::vector<uint32>>	_outs = std::make_shared<std::vector<uint32>>();

			for (auto i = 0; i < map.w; i++)
				for (auto j = 0; j < map.h; j++)
				{
					Cell& cell = map[i][j];

					if (!cell.highlight)
						continue;

					_inds->push_back((uint32)_vtxs->size());
					_vtxs->push_back(map.vtxs[cell.v0]);
					_inds->push_back((uint32)_vtxs->size());
					_vtxs->push_back(map.vtxs[cell.v1]);
					_inds->push_back((uint32)_vtxs->size());
					_vtxs->push_back(map.vtxs[cell.v2]);
					_inds->push_back((uint32)_vtxs->size());
					_vtxs->push_back(map.vtxs[cell.v3]);

					// TODO: Add outline algorithm
				};

			vtxs.exchange(_vtxs);
			inds.exchange(_inds);
			outs.exchange(_outs);
		});
		thread.detach();
	};
	void Highlights::load()
	{
		_mvp = glGetUniformLocation(map.shader.id, "mvp");
		_color = glGetUniformLocation(map.shader.id, "color");
	};
	void Highlights::render()
	{
		std::shared_ptr<std::vector<vec4>>		_vtxs = vtxs.load();
		std::shared_ptr<std::vector<uint32>>	_inds = inds.load();
		std::shared_ptr<std::vector<uint32>>	_outs = outs.load();

		if (_vtxs && _inds && _outs)
		{
			glUseProgram(map.shader.id);
			glEnableVertexAttribArray(VEC4_0_16);
			glUniformMatrix4fv(_mvp, 1, GL_FALSE, &Camera::mvp[0][0]);

			glBufferData(
				GL_ARRAY_BUFFER,
				_vtxs->size() * sizeof(vec4),
				_vtxs->data(),
				GL_DYNAMIC_DRAW);

			glLineWidth(20);

			glUniform4fv(_color, 1, &vec4(0, 0, .5, .5)[0]);
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				_inds->size() * sizeof(uint32),
				_inds->data(),
				GL_DYNAMIC_DRAW);
			glDrawElements(GL_QUADS, (GLsizei)_inds->size(), GL_UNSIGNED_INT, nullptr);
			
			glUniform4fv(_color, 1, &vec4(0, 0, 0, 1)[0]);
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
