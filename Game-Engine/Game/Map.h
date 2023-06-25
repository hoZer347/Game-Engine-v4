#pragma once

#include "../Data.h"
#include "../Enums.h"
#include "../Engine.h"
#include "../Camera.h"
#include "../Shader.h"
#include "../Control.h"
#include "../Timing.h"
#include "Unit.h"
#include "Cell.h"
#include "Terrain.h"

#include <math.h>

namespace loom
{
	template <
		auto x_size,				auto y_size,
		auto x0_buffer = 0,			auto y0_buffer = 0,
		auto x1_buffer = x0_buffer,	auto y1_buffer = y0_buffer>
	struct alignas(32) Map final :
		virtual public Updateable,
		virtual public Renderable
	{
		static_assert(x_size > 0, "x_size must be > 0");
		static_assert(y_size > 0, "y_size must be > 0");

		// Determines the heights of each cell in terms of z = height_func(x, y)
		float(*height_func)(float x, float y) = [](float, float) { return 1.f; };

		// Sets heights at (x0, y0) -> (x1, y1) to the given value
		inline void fill_heights(auto value, auto x0, auto y0, auto x1 = x0, auto y1 = y0)
		{
			for (auto i = std::max(x0, 0); i < x1 && i < x_size; i++)
				for (auto j = std::max(y0, 0); j < y1 && j < y_size; j++)
					for (auto k = 0; k < 4; k++)
						vtxs[i][j][k].z = value;
		};
		// Sets terrain at (x0, y0) -> (x1, y1) to the given value
		inline void fill_terrain(Terrain& terrain, auto x0, auto y0, auto x1 = x0, auto y1 = y0)
		{
			for (auto i = std::max(x0, 0); i < x1 && i < x_size; i++)
				for (auto j = std::max(y0, 0); j < y1 && j < y_size; j++)
					terrains[i][j] = terrain;
		};
		// Connects 2 Cells at a given direction
		inline void connect(auto x0, auto y0, auto d0, auto x1, auto y1, auto d1)
		{
			// TODO:
		};
		// Connects adjacent cells (x0, y0) -> (x1, y1) to adjecent cells
		inline void connect_adjacent(auto x0, auto y0, auto x1 = x0, auto y1 = y1)
		{
			// TODO:
		};
		// Disonnects adjacent cells (x0, y0) -> (x1, y1) from adjecent cells
		inline void disconnect_adjacent(auto x0, auto y0, auto x1 = x0, auto y1 = y0)
		{
			// TODO:
		};
		// Adds a given unit to the map at (x, y)
		inline void set_unit(Unit* unit, auto x, auto y)
		{
			cells[x][y].unit = unit;
		};
		// Generates a mesh based on the given changes
		inline void generate()
		{
			// Vertices
			for (auto i = 0; i < x_size + 1; i++)
				for (auto j = 0; j < y_size + 1; j++)
					if (terrains[i][j].id)
					{
						vtxs[i][j][0] = vec4(i,     j,     0, 1);
						vtxs[i][j][1] = vec4(i + 1, j,     0, 1);
						vtxs[i][j][2] = vec4(i + 1, j + 1, 0, 1);
						vtxs[i][j][3] = vec4(i,     j + 1, 0, 1);

						vtxs[i][j][4] = vec4(i,     j,     0, 1);
					}
					else
					{
						vtxs[i][j][0] = vec4(0, 0, 0, 1);
						vtxs[i][j][1] = vec4(0, 0, 0, 1);
						vtxs[i][j][2] = vec4(0, 0, 0, 1);
						vtxs[i][j][3] = vec4(0, 0, 0, 1);

						vtxs[i][j][4] = vec4(i, j, 0, 1);
					};

			// Grid Outline Indices
			for (auto i = y0_buffer; i < x_size - x1_buffer; i++)
				for (auto j = y0_buffer; j < y_size - y1_buffer; j++)
					if (terrains[i][j].id)
					{
						inds[i - x0_buffer][j - y0_buffer][0] = (uint32)((uint64)&vtxs[i][j][0] - (uint64)&vtxs) / sizeof(vec4);
						inds[i - x0_buffer][j - y0_buffer][1] = (uint32)((uint64)&vtxs[i][j][1] - (uint64)&vtxs) / sizeof(vec4);
						inds[i - x0_buffer][j - y0_buffer][2] = (uint32)((uint64)&vtxs[i][j][1] - (uint64)&vtxs) / sizeof(vec4);
						inds[i - x0_buffer][j - y0_buffer][3] = (uint32)((uint64)&vtxs[i][j][2] - (uint64)&vtxs) / sizeof(vec4);
						inds[i - x0_buffer][j - y0_buffer][4] = (uint32)((uint64)&vtxs[i][j][2] - (uint64)&vtxs) / sizeof(vec4);
						inds[i - x0_buffer][j - y0_buffer][5] = (uint32)((uint64)&vtxs[i][j][3] - (uint64)&vtxs) / sizeof(vec4);
						inds[i - x0_buffer][j - y0_buffer][6] = (uint32)((uint64)&vtxs[i][j][3] - (uint64)&vtxs) / sizeof(vec4);
						inds[i - x0_buffer][j - y0_buffer][7] = (uint32)((uint64)&vtxs[i][j][0] - (uint64)&vtxs) / sizeof(vec4);

						inds[i - x0_buffer][j - y0_buffer][8] = (uint32)((uint64)&vtxs[i][j][0] - (uint64)&vtxs) / sizeof(vec4);
						inds[i - x0_buffer][j - y0_buffer][9] = (uint32)((uint64)&vtxs[i][j][4] - (uint64)&vtxs) / sizeof(vec4);
						inds[i - x0_buffer][j - y0_buffer][10] = (uint32)((uint64)&vtxs[i][j][1] - (uint64)&vtxs) / sizeof(vec4);
						inds[i - x0_buffer][j - y0_buffer][11] = (uint32)((uint64)&vtxs[i + 1][j][4] - (uint64)&vtxs) / sizeof(vec4);
						inds[i - x0_buffer][j - y0_buffer][12] = (uint32)((uint64)&vtxs[i][j][2] - (uint64)&vtxs) / sizeof(vec4);
						inds[i - x0_buffer][j - y0_buffer][13] = (uint32)((uint64)&vtxs[i + 1][j + 1][4] - (uint64)&vtxs) / sizeof(vec4);
						inds[i - x0_buffer][j - y0_buffer][14] = (uint32)((uint64)&vtxs[i][j][3] - (uint64)&vtxs) / sizeof(vec4);
						inds[i - x0_buffer][j - y0_buffer][15] = (uint32)((uint64)&vtxs[i][j + 1][4] - (uint64)&vtxs) / sizeof(vec4);
					};

			// Texture Indices
			for (auto i = 0; i < x_size + 1; i++)
				for (auto j = 0; j < y_size + 1; j++)
					if (terrains[i][j].id)
					{
						tnds[i][j][0] = (uint32)((uint64)&vtxs[i][j][0] - (uint64)&vtxs) / sizeof(vec4);
						tnds[i][j][1] = (uint32)((uint64)&vtxs[i][j][1] - (uint64)&vtxs) / sizeof(vec4);
						tnds[i][j][2] = (uint32)((uint64)&vtxs[i][j][2] - (uint64)&vtxs) / sizeof(vec4);
						tnds[i][j][3] = (uint32)((uint64)&vtxs[i][j][3] - (uint64)&vtxs) / sizeof(vec4);
					};
		};

		bool draw_grid = true;
		vec4 grid_color = vec4(1, 0, 0, .5);

	private:
		vec4	vtxs[x_size + 1][y_size + 1][5]{};
		uint32	inds[x_size-x0_buffer-x1_buffer][y_size-y0_buffer-y1_buffer][16]{};
		uint32	tnds[x_size][y_size][4]{};
		uint32	hlts[x_size][y_size]{};
		Terrain	terrains[x_size][y_size]{};
		Cell	cells[x_size][y_size]{};

		void update() override
		{
			static Timer timer;

			if (timer.GetDiff_mls() < 1000.f / 60)
				return;
			timer.push(std::chrono::milliseconds(1000 / 60));

			static float _i = 0;
			for (auto i = 0; i < x_size; i++)
				for (auto j = 0; j < y_size; j++)
				{
					float height = height_func((float)i, (float)j);

					for (auto k = 0; k < 4; k++)
						vtxs[i][j][k].z = height;

					if (Unit* unit = cells[i][j].unit)
						unit->pos = vec4(i, j, height, 1);
				};
			_i++;
		};
		void render() override
		{
			glEnableVertexAttribArray(VEC4_0_16);

			glBufferData(
				GL_ARRAY_BUFFER,
				sizeof(vtxs),
				vtxs,
				GL_DYNAMIC_DRAW);

			
			// Drawing grid outline
			if (draw_grid)
			{
				glUseProgram(shader.id);

				glUniformMatrix4fv(glGetUniformLocation(shader.id, "mvp"), 1, false, (float*)&Camera::mvp);
				glUniform4fv(glGetUniformLocation(shader.id, "color"), 1, (float*)&grid_color);

				glBufferData(
					GL_ELEMENT_ARRAY_BUFFER,
					sizeof(inds),
					inds,
					GL_DYNAMIC_DRAW);

				glDrawElements(GL_LINES, (GLsizei)sizeof(inds), GL_UNSIGNED_INT, nullptr);
			};


			// Drawing textures onto the grid
			glUseProgram(_shader.id);

			glUniformMatrix4fv(glGetUniformLocation(_shader.id, "mvp"), 1, false, (float*)&Camera::mvp);

			glBindTexture(GL_TEXTURE_2D, texture.id);

			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				sizeof(tnds),
				tnds,
				GL_DYNAMIC_DRAW);

			glDrawElements(GL_QUADS, (GLsizei)sizeof(tnds), GL_UNSIGNED_INT, nullptr);



			glBindTexture(GL_TEXTURE_2D, 0);
			glDisableVertexAttribArray(VEC4_0_16);
		};

		Texture texture{ "Resources/stone.png", GL_RGB };
		Shader shader{"Game/Map"};
		Shader _shader{"Game/MapTexture"};
	};
};
