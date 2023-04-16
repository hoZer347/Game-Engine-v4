#pragma once

#include "Data.h"

#include <glm/glm.hpp>
using namespace glm;

#include <memory>

namespace loom
{
	struct Cell
	{
		const ivec2 pos;
		uint32_t v0 = -1, v1 = v0, v2 = v0, v3 = v0;
	};

	struct Grid final : virtual private Loadable, virtual private Updateable, virtual private Renderable
	{
	private:
		struct Column
		{
			vec4* ptr;
			vec4& operator[](uint16_t i) { return ptr[i]; }
		};

		std::vector<vec4> vtxs;
		std::vector<uint32_t> inds;

		uint32_t x_size, y_size;
		uint32_t _trns = 0, _mvp = 0;
		
		std::vector<std::vector<Cell>> cells;

		vec4 mpos = vec4(0);

	public:
		Grid(uint32_t x_size, uint32_t y_size);
		virtual ~Grid();

		// Initializes cells from (x0, y0) to (x1, y1)
		// Connected means whether or not they share vertices
		void ConstructCells(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, bool connected = true);
		std::vector<Cell>& operator[](uint32_t i) { return cells[i]; };

	protected:
		friend struct Utils;
		void load() override;
		void update() override;
		void render() override;
	};
};
