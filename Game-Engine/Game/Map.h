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
#include "glm/glm.hpp"
#include "glm/gtx/intersect.hpp"
using namespace glm;

#include <math.h>
#include <unordered_set>

namespace loom
{
	template <
		auto x_size,				auto y_size,
		auto x0_buffer = 0,			auto y0_buffer = x0_buffer,
		auto x1_buffer = x0_buffer,	auto y1_buffer = y0_buffer>
	struct alignas(32) Map final :
		virtual public Updateable,
		virtual public Renderable
	{
		static_assert(x_size > 0, "x_size must be > 0");
		static_assert(y_size > 0, "y_size must be > 0");

		typedef vec4 Vtx;

		Map()
		{
			for (auto i = 0; i < x_size - x0_buffer - x1_buffer; i++)
				for (auto j = 0; j < y_size - y0_buffer - y1_buffer; j++)
					cells[i][j].unit = nullptr;
		};

		// Determines the heights of each cell in terms of z = height_func(x, y)
		float(*height_func)(float x, float y) = [](float, float) { return 1.f; };

		// Sets terrain at (x0, y0) -> (x1, y1) to the given value
		inline void fill_terrain(auto terrain, auto x0, auto y0, auto x1 = x0, auto y1 = y0)
		{
			//for (auto i = std::max(x0, 0); i < x1 && i < x_size; i++)
			//	for (auto j = std::max(y0, 0); j < y1 && j < y_size; j++)
			//		terrains[i][j] = terrain;
		};
		inline void clear_highlights()
		{
			for (auto i = 0; i < x_size - x0_buffer - x1_buffer; i++)
				for (auto j = 0; j < y_size - y0_buffer - y1_buffer; j++)
					cells[i][j].highlight = 0;

			refresh_highlights();
		};
		// Sets highlights (x0, y0) -> (x1, y1) to the given value
		inline void fill_highlights(auto value, auto x0, auto y0, auto x1 = x0, auto y1 = y0)
		{
			for (auto i = std::max(x0, 0); i < x1 && i < x_size - x0_buffer - x1_buffer; i++)
				for (auto j = std::max(y0, 0); j < y1 && j < y_size - y0_buffer - y1_buffer; j++)
					cells[i][j].highlight = value;

			refresh_highlights();
		};
		// Returns a pointer to the cell at (x, y)
		inline Cell* get_cell_ptr(auto x, auto y)
		{
			return &cells[x][y];
		};
		// Floodfills from (x, y) using cell connections (not adjacency)
		inline void floodfill_hightlights(const uint8& value, float amount, Cell* c, std::unordered_set<Cell*>&& cells = { })
		{
			cells.insert(c);

			amount = std::min(amount, (float)(x_size - x0_buffer - x1_buffer) * (y_size - y0_buffer - y1_buffer));

			for (; amount > 0; amount--)
			{
				std::unordered_set<Cell*> _cells;

				for (auto cell : cells)
					if (cell)
					{
						if (cell->U) _cells.insert(cell->U);
						if (cell->D) _cells.insert(cell->D);
						if (cell->L) _cells.insert(cell->L);
						if (cell->R) _cells.insert(cell->R);
					};

				for (auto cell : _cells)
					cells.insert(cell);

				_cells.clear();
			};

			for (auto& cell : cells)
				if (cell)
					cell->highlight = value;

			refresh_highlights();
		};
		// Connects 2 Cells at a given direction
		inline void connect(auto x0, auto y0, auto d0, auto x1, auto y1, auto d1)
		{
			// TODO:
		};
		// Connects all 
		inline void connect_height_adjacent(auto max_height_diff = .2f)
		{
			// TODO:
		};
		// Connects adjacent cells (x0, y0) -> (x1, y1) to adjecent cells
		inline void connect_adjacent(auto x0, auto y0, auto x1 = x0, auto y1 = y1)
		{
			for (auto i = std::max(x0, 0); i < x1 && i < x_size - x0_buffer - x1_buffer; i++)
				for (auto j = std::max(y0, 0); j < y1 && j < y_size - y0_buffer - y1_buffer; j++)
				{
					if (j < y_size - y0_buffer - y1_buffer - 1)	cells[i][j].U = &cells[i][j + 1];
					if (j > 0)									cells[i][j].D = &cells[i][j - 1];
					if (i > 0)									cells[i][j].L = &cells[i - 1][j];
					if (i < x_size - x0_buffer - x1_buffer - 1)	cells[i][j].R = &cells[i + 1][j];
				};
		};
		// Disonnects adjacent cells (x0, y0) -> (x1, y1) from adjecent cells
		inline void disconnect(auto x0, auto y0, auto x1 = x0, auto y1 = y0)
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
				{
					vtxs[i][j][0] = vec4(i,     j,     0, vtxs[i][j][0].w);
					vtxs[i][j][1] = vec4(i + 1, j,     0, vtxs[i][j][1].w);
					vtxs[i][j][2] = vec4(i + 1, j + 1, 0, vtxs[i][j][2].w);
					vtxs[i][j][3] = vec4(i,     j + 1, 0, vtxs[i][j][3].w);

					vtxs[i][j][4] = vec4(i,     j,     0, vtxs[i][j][4].w);
				};


			// Grid Outline Indices
			for (auto i = x0_buffer; i < x_size - x1_buffer; i++)
				for (auto j = y0_buffer; j < y_size - y1_buffer; j++)
				{
					inds[i - x0_buffer][j - y0_buffer][0] = get_vtx_ind(i, j, 0);
					inds[i - x0_buffer][j - y0_buffer][1] = get_vtx_ind(i, j, 1);
					inds[i - x0_buffer][j - y0_buffer][2] = get_vtx_ind(i, j, 1);
					inds[i - x0_buffer][j - y0_buffer][3] = get_vtx_ind(i, j, 2);
					inds[i - x0_buffer][j - y0_buffer][4] = get_vtx_ind(i, j, 2);
					inds[i - x0_buffer][j - y0_buffer][5] = get_vtx_ind(i, j, 3);
					inds[i - x0_buffer][j - y0_buffer][6] = get_vtx_ind(i, j, 3);
					inds[i - x0_buffer][j - y0_buffer][7] = get_vtx_ind(i, j, 0);

					inds[i - x0_buffer][j - y0_buffer][8] = get_vtx_ind(i, j, 0);
					inds[i - x0_buffer][j - y0_buffer][9] = get_vtx_ind(i, j, 4);
					inds[i - x0_buffer][j - y0_buffer][10] = get_vtx_ind(i, j, 1);
					inds[i - x0_buffer][j - y0_buffer][11] = get_vtx_ind(i + 1, j, 4);
					inds[i - x0_buffer][j - y0_buffer][12] = get_vtx_ind(i, j, 2);
					inds[i - x0_buffer][j - y0_buffer][13] = get_vtx_ind(i + 1, j + 1, 4);
					inds[i - x0_buffer][j - y0_buffer][14] = get_vtx_ind(i, j, 3);
					inds[i - x0_buffer][j - y0_buffer][15] = get_vtx_ind(i, j + 1, 4);
				};


			// Texture Indices
			for (auto i = 0; i < x_size + 1; i++)
				for (auto j = 0; j < y_size + 1; j++)
				{
					tnds[i][j][0] = get_vtx_ind(i, j, 0);
					tnds[i][j][1] = get_vtx_ind(i, j, 1);
					tnds[i][j][2] = get_vtx_ind(i, j, 2);
					tnds[i][j][3] = get_vtx_ind(i, j, 3);
				};


			// Resetting Highlights
			for (auto i = 0; i < x_size - x0_buffer - x1_buffer; i++)
				for (auto j = 0; j < y_size - y0_buffer - y1_buffer; j++)
					cells[i][j].highlight = 0;
		};
		// Detects highlights on the map cells and generates an index list based on it
		inline void refresh_highlights()
		{
			std::scoped_lock<std::recursive_mutex> lock0{GameObject<Updateable>::mut};
			std::scoped_lock<std::recursive_mutex> lock1{GameObject<Renderable>::mut};

			hlts.clear();
			for (auto i = x0_buffer; i < x_size - x1_buffer; i++)
				for (auto j = y0_buffer; j < y_size - y1_buffer; j++)
					if (uint8 highlight = cells[i - x0_buffer][j - y0_buffer].highlight)
					{
						vtxs[i][j][0].w = (float)highlight;
						vtxs[i][j][1].w = (float)highlight;
						vtxs[i][j][2].w = (float)highlight;
						vtxs[i][j][3].w = (float)highlight;

						hlts.push_back(get_vtx_ind(i, j, 0));
						hlts.push_back(get_vtx_ind(i, j, 1));
						hlts.push_back(get_vtx_ind(i, j, 2));
						hlts.push_back(get_vtx_ind(i, j, 3));
					};
		};
		// Sets heights at (x0, y0) -> (x1, y1) to the given value
		inline void fill_heights(auto value, auto x0, auto y0, auto x1 = x0, auto y1 = y0)
		{
			for (auto i = std::max(x0, 0); i < x1 && i < x_size; i++)
				for (auto j = std::max(y0, 0); j < y1 && j < y_size; j++)
					for (auto k = 0; k < 4; k++)
						vtxs[i][j][k].z = value;
		};

		bool draw_outline = true;
		vec4 outline_color = vec4(1, 0, 0, .5);
		vec4 hovered_color = vec4(.5, 0, 0, 1);

	//private:
		Vtx		vtxs[x_size + 1][y_size + 1][5]{};
		uint32	inds[x_size-x0_buffer-x1_buffer][y_size-y0_buffer-y1_buffer][16]{};
		uint32	tnds[x_size][y_size][4]{};
		uint32	hvrd[4]{ 0, 0, 0, 0 };
		Cell	cells[x_size-x0_buffer-x1_buffer][y_size-y0_buffer-y1_buffer]{};

		std::vector<uint32> hlts;

		uint32 inline get_vtx_ind(auto x, auto y, auto offset=0)
		{
			return (uint32)((uint64)&vtxs[x][y][offset] - (uint64)&vtxs) / sizeof(Vtx);
		};

		void update() override
		{
			static bool finished = true;

			if (!finished) return;

			static Timer timer;
			
			if (timer.GetDiff_mls() < 1000.f / 60)
				return;
			timer.push(std::chrono::milliseconds(1000 / 60));

			finished = false;

			std::thread thread([this]()
			{
				// Updating Heights
				for (auto i = 0; i < x_size; i++)
					for (auto j = 0; j < y_size; j++)
					{
						float height = height_func((float)i, (float)j);

						for (auto k = 0; k < 4; k++)
							vtxs[i][j][k].z = height;
					};


				// Updating Unit Positions
				for (auto i = 0; i < x_size - x0_buffer - x1_buffer; i++)
					for (auto j = 0; j < y_size - y0_buffer - y1_buffer; j++)
						if (Unit* unit = cells[i][j].unit)
							/*unit->pos = vtxs[i + x0_buffer][j + y0_buffer][0]*/;


				// Updating Hovered Cell
				vec2 buh = vec2(0, 0);
				float d = 0;
				for (auto i = x0_buffer; i < x_size - x1_buffer; i++)
					for (auto j = y0_buffer; j < y_size - y1_buffer; j++)
						if (intersectRayTriangle(
								vec3(Camera::mpos),
								vec3(Camera::mdir),
								vec3(vtxs[i][j][0]),
								vec3(vtxs[i][j][1]),
								vec3(vtxs[i][j][2]),
								buh, d) ||
							intersectRayTriangle(
								vec3(Camera::mpos),
								vec3(Camera::mdir),
								vec3(vtxs[i][j][0]),
								vec3(vtxs[i][j][3]),
								vec3(vtxs[i][j][2]),
								buh, d))
						{
							hvrd[0] = get_vtx_ind(i, j, 0);
							hvrd[1] = get_vtx_ind(i, j, 1);
							hvrd[2] = get_vtx_ind(i, j, 2);
							hvrd[3] = get_vtx_ind(i, j, 3);

							goto _break;
						};

				_break:

				finished = true;
			});
			thread.detach();
		};
		void render() override
		{
			glEnableVertexAttribArray(VEC4_0_16);

			glBufferData(
				GL_ARRAY_BUFFER,
				sizeof(vtxs),
				vtxs,
				GL_DYNAMIC_DRAW);


			// Drawing textures onto the grid
			glUseProgram(texture_shader->id);
			glUniformMatrix4fv(glGetUniformLocation(texture_shader->id, "mvp"), 1, false, (float*)&Camera::mvp);
			glBindTexture(GL_TEXTURE_2D, texture->id);
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				sizeof(tnds),
				tnds,
				GL_DYNAMIC_DRAW);
			glDrawElements(GL_QUADS, (GLsizei)sizeof(tnds), GL_UNSIGNED_INT, nullptr);


			// Drawing outlines
			if (draw_outline)
			{
				glUseProgram(outline_shader->id);
				glUniformMatrix4fv(glGetUniformLocation(outline_shader->id, "mvp"), 1, false, (float*)&Camera::mvp);
				glUniform4fv(glGetUniformLocation(outline_shader->id, "color"), 1, &outline_color[0]);
				glBufferData(
					GL_ELEMENT_ARRAY_BUFFER,
					sizeof(inds),
					inds,
					GL_DYNAMIC_DRAW);
				glDrawElements(GL_LINES, (GLsizei)sizeof(inds), GL_UNSIGNED_INT, nullptr);
			};

			
			// Drawing highlights
			glUseProgram(highlight_shader->id);
			glUniformMatrix4fv(glGetUniformLocation(highlight_shader->id, "mvp"), 1, false, (float*)&Camera::mvp);
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				hlts.size() * sizeof(uint32),
				hlts.data(),
				GL_DYNAMIC_DRAW);
			glDrawElements(GL_QUADS, (GLsizei)hlts.size() * sizeof(uint32), GL_UNSIGNED_INT, nullptr);


			// Drawing the Hovered Cell
			glUseProgram(hovered_shader->id);
			glUniformMatrix4fv(glGetUniformLocation(hovered_shader->id, "mvp"), 1, false, (float*)&Camera::mvp);
			glUniform4fv(glGetUniformLocation(hovered_shader->id, "color"), 1, &hovered_color[0]);
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				sizeof(hvrd),
				hvrd,
				GL_DYNAMIC_DRAW);
			glDrawElements(GL_QUADS, (GLsizei)sizeof(hvrd), GL_UNSIGNED_INT, nullptr);


			glBindTexture(GL_TEXTURE_2D, 0);
			glDisableVertexAttribArray(VEC4_0_16);
		};

		ptr<Texture> texture{ "Resources/stone.png", GL_RGB };
		ptr<Shader> texture_shader{ "Game/MapTexture" };
		ptr<Shader> outline_shader{ "Game/MapOutline" };
		ptr<Shader> highlight_shader{ "Game/MapHighlights" };
		ptr<Shader> hovered_shader{ "Game/MapHovered" };
	};
};
