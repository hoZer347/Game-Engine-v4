#pragma once

#include "Data.h"
#include "Geometry.h"

namespace loom
{
	struct Cell final : public Object
	{
		Cell(mat4& trns, uint32_t x, uint32_t y)
		: trns(trns), pos(x, y)
		{ };

	private:
		void load() override;
		void unload() override;
		
		mat4& trns;
		vec2 pos;
	};



	struct Grid final : public Renderable
	{
		Grid(uint32_t x_size, uint32_t y_size);

		Squares squares;

	private:
		void render() override;

		uint32_t x_size;
		uint32_t y_size;
	};
};
