#pragma once

#include "../Data.h"
#include "../Enums.h"
#include "../Engine.h"
#include "../Camera.h"
#include "../Shader.h"
#include "../Timing.h"
#include "../Animation.h"
#include "Unit.h"
#include "Cell.h"
#include "Terrain.h"
#include "MapImpl.h"
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
	struct Map final :
		public MapImpl,
		virtual public Updateable,
		virtual public Renderable
	{
		static_assert(x_size > 0, "x_size must be > 0");
		static_assert(y_size > 0, "y_size must be > 0");

		bool draw_outline = true;
		vec4 outline_color = vec4(1, 0, 0, .5);
		vec4 hovered_color = vec4(.5, 0, 0, 1);

		typedef vec4 Vtx;

		void clear_highlights() override
		{
			for (auto i = 0; i < x_size - x0_buffer - x1_buffer; i++)
				for (auto j = 0; j < y_size - y0_buffer - y1_buffer; j++)
					cells[i][j].highlight = 0;

			refresh_highlights();
		};
		void fill_highlights(uint16 value, uint16 x0, uint16 y0, uint16 x1 = x0, uint16 y1 = y0) override
		{
			for (auto i = x0; i < x1 && i < x_size - x0_buffer - x1_buffer; i++)
				for (auto j = y0; j < y1 && j < y_size - y0_buffer - y1_buffer; j++)
					cells[i][j].highlight = (uint8)value;

			refresh_highlights();
		};
		void floodfill_hightlights(const uint8& value, float amount, std::unordered_set<Cell*>& cells = { })
		{
			if (cells.size() == 1)
				(*cells.begin())->highlight = value;

			amount = std::min(amount, (float)(x_size - x0_buffer - x1_buffer) * (y_size - y0_buffer - y1_buffer));

			for (; amount > 0; amount--)
			{
				std::unordered_set<Cell*> _cells = cells;

				for (auto& cell : _cells)
					if (cell)
					{
						if (cell->U && !cell->U->unit && _cells.find(cell->U) == _cells.end()) { cells.insert(cell->U); cell->U->highlight = value; };
						if (cell->D && !cell->D->unit && _cells.find(cell->D) == _cells.end()) { cells.insert(cell->D); cell->D->highlight = value; };
						if (cell->L && !cell->L->unit && _cells.find(cell->L) == _cells.end()) { cells.insert(cell->L); cell->L->highlight = value; };
						if (cell->R && !cell->R->unit && _cells.find(cell->R) == _cells.end()) { cells.insert(cell->R); cell->R->highlight = value; };
					};
			};

			refresh_highlights();
		};
		void connect_adjacent(uint16 x0, uint16 y0, uint16 x1 = x0, uint16 y1 = y1) override
		{
			for (uint16 i = x0; i < x1 && i < x_size - x0_buffer - x1_buffer; i++)
				for (uint16 j = y0; j < y1 && j < y_size - y0_buffer - y1_buffer; j++)
				{
					if (j < y_size - y0_buffer - y1_buffer - 1)	cells[i][j].U = &cells[i][j + 1];
					if (j > 0)									cells[i][j].D = &cells[i][j - 1];
					if (i > 0)									cells[i][j].L = &cells[i - 1][j];
					if (i < x_size - x0_buffer - x1_buffer - 1)	cells[i][j].R = &cells[i + 1][j];
				};
		};
		void set_unit(ptr<Unit> unit, uint16 x, uint16 y) override
		{
			cells[x][y].unit = unit;
			unit->cell = &cells[x][y];
			unit->pos = vtxs[x + x0_buffer][y + y0_buffer][0];
		};
		void generate() override
		{
			// Vertices
			for (uint16 i = 0; i < x_size + 1; i++)
				for (uint16 j = 0; j < y_size + 1; j++)
				{
					vtxs[i][j][0] = vec4(i,     j,     0, vtxs[i][j][0].w);
					vtxs[i][j][1] = vec4(i + 1, j,     0, vtxs[i][j][1].w);
					vtxs[i][j][2] = vec4(i + 1, j + 1, 0, vtxs[i][j][2].w);
					vtxs[i][j][3] = vec4(i,     j + 1, 0, vtxs[i][j][3].w);

					vtxs[i][j][4] = vec4(i,     j,     0, vtxs[i][j][4].w);
				};


			// Grid Outline Indices
			for (uint16 i = x0_buffer; i < x_size - x1_buffer; i++)
				for (uint16 j = y0_buffer; j < y_size - y1_buffer; j++)
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
			for (uint16 i = 0; i < x_size; i++)
				for (uint16 j = 0; j < y_size; j++)
				{
					// Top Face
					tnds[i][j][0]  = get_vtx_ind(i, j, 0);
					tnds[i][j][1]  = get_vtx_ind(i, j, 1);
					tnds[i][j][2]  = get_vtx_ind(i, j, 2);
					tnds[i][j][3]  = get_vtx_ind(i, j, 0);
					tnds[i][j][4]  = get_vtx_ind(i, j, 2);
					tnds[i][j][5]  = get_vtx_ind(i, j, 3);

					// South Face
					tnds[i][j][8] = get_vtx_ind(i, j, 0);
					tnds[i][j][7] = get_vtx_ind(i, j, 1);
					tnds[i][j][6] = get_vtx_ind(i, j, 4);
					tnds[i][j][11] = get_vtx_ind(i, j, 1);
					tnds[i][j][10] = get_vtx_ind(i+1, j, 4);
					tnds[i][j][9] = get_vtx_ind(i, j, 4);

					// East Face
					tnds[i][j][14] = get_vtx_ind(i, j, 1);
					tnds[i][j][13] = get_vtx_ind(i, j, 2);
					tnds[i][j][12] = get_vtx_ind(i+1, j, 4);
					tnds[i][j][17] = get_vtx_ind(i, j, 2);
					tnds[i][j][16] = get_vtx_ind(i + 1, j + 1, 4);
					tnds[i][j][15] = get_vtx_ind(i + 1, j, 4);

					// North Face
					tnds[i][j][20] = get_vtx_ind(i, j, 2);
					tnds[i][j][19] = get_vtx_ind(i, j, 3);
					tnds[i][j][18] = get_vtx_ind(i, j + 1, 4);
					tnds[i][j][23] = get_vtx_ind(i, j + 1, 4);
					tnds[i][j][22] = get_vtx_ind(i+1, j+1, 4);
					tnds[i][j][21] = get_vtx_ind(i, j, 2);

					// West Face
					tnds[i][j][26] = get_vtx_ind(i, j, 3);
					tnds[i][j][25] = get_vtx_ind(i, j, 0);
					tnds[i][j][24] = get_vtx_ind(i, j, 4);
					tnds[i][j][29] = get_vtx_ind(i, j, 4);
					tnds[i][j][28] = get_vtx_ind(i, j+1, 4);
					tnds[i][j][27] = get_vtx_ind(i, j, 3);
				};


			// Resetting Highlights
			for (uint16 i = 0; i < x_size - x0_buffer - x1_buffer; i++)
				for (uint16 j = 0; j < y_size - y0_buffer - y1_buffer; j++)
				{
					cells[i][j].highlight = 0;
					cells[i][j].pos = &vtxs[i + x0_buffer][j + y0_buffer][0];
				};
		};
		void refresh_highlights() override
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
		void fill_heights(uint16 value, uint16 x0, uint16 y0, uint16 x1 = x0, uint16 y1 = y0) override
		{
			for (uint16 i = x0; i < x1 && i < x_size; i++)
				for (uint16 j = y0; j < y1 && j < y_size; j++)
					for (uint16 k = 0; k < 4; k++)
						vtxs[i][j][k].z = value;
		};
		void find_hovered() override
		{
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

						hovered = &cells[i - x0_buffer][j - y0_buffer];

						return;
					};
		};

	protected:
		friend struct ptr<Map<x_size, y_size, x0_buffer, y0_buffer, x1_buffer, y1_buffer>>;
		Map() { };

	private:
		// Returns a pointer to the cell at (x, y)
		Cell* get_cell_ptr(uint16 x, uint16 y)
		{
			return &cells[x][y];
		};

		Cell	cells[x_size - x0_buffer - x1_buffer][y_size - y0_buffer - y1_buffer]{};
		Vtx		vtxs[x_size + 1][y_size + 1][5]{};
		uint32	inds[x_size-x0_buffer-x1_buffer][y_size-y0_buffer-y1_buffer][16]{};
		uint32	tnds[x_size][y_size][30]{};
		uint32	hvrd[4]{ 0, 0, 0, 0 };
		uint64 time_elapsed = 0;

		std::vector<uint32> hlts;

		uint32 inline get_vtx_ind(auto x, auto y, auto offset=0)
		{
			return (uint32)((uint64)&vtxs[x][y][offset] - (uint64)&vtxs) / sizeof(Vtx);
		};

		void update() override
		{
			static Timer timer;

			if (timer.GetDiff_mls() < 1000.f / 60)
				return;
			timer.push(std::chrono::milliseconds(1000 / 60));

			time_elapsed++;

			// Updating Heights
			for (auto i = 0; i < x_size; i++)
				for (auto j = 0; j < y_size; j++)
				{
					float height = height_func((float)i, (float)j, time_elapsed);

					for (auto k = 0; k < 4; k++)
						vtxs[i][j][k].z = height;
				};


			// Updating Unit Positions
			for (auto i = 0; i < x_size - x0_buffer - x1_buffer; i++)
				for (auto j = 0; j < y_size - y0_buffer - y1_buffer; j++)
					if (ptr<Unit> unit = cells[i][j].unit)
						unit->pos.z = height_func(unit->pos.x, unit->pos.y, time_elapsed);
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
			glDrawElements(GL_TRIANGLES, (GLsizei)sizeof(tnds), GL_UNSIGNED_INT, nullptr);


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

	public:
		ptr<Texture> texture{ 0, "Resources/stone.png", GL_RGB };
		ptr<Shader> texture_shader{ 0, "Game/MapTexture" };
		ptr<Shader> outline_shader{ 0, "Game/MapOutline" };
		ptr<Shader> highlight_shader{ 0, "Game/MapHighlights" };
		ptr<Shader> hovered_shader{ 0, "Game/MapHovered" };
	};
};
