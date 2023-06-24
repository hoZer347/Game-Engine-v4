#pragma once

#include "../Data.h"
#include "../Enums.h"
#include "../Camera.h"
#include "../Shader.h"
#include "../Serialization.h"
#include "Unit.h"


namespace loom
{
	struct Cell final
	{
		uvec2 pos;
		Unit *unit = nullptr;
		Cell *U = nullptr, *D = nullptr, *L = nullptr, *R = nullptr;
	};
	template <uint8 x_size, uint8 y_size>
	struct Map :
		virtual public Updateable,
		virtual public Renderable
	{
		static_assert(x_size > 0, "x_size must be > 0");
		static_assert(y_size > 0, "y_size must be > 0");

		Map(const char* file_name = "Game/default.map") :
			file_name(file_name)
		{
			vtxs.reserve(x_size * y_size);
			for (auto i = 0; i < x_size + 1; i++)
				for (auto j = 0; j < y_size + 1; j++)
					vtxs.emplace_back(i, j, 0, 1);

			for (auto i = 0; i < x_size; i++)
				for (auto j = 0; j < y_size; j++)
				{
					heights[i][j] = 0;
					highlights[i][j] = 0;
					cells[i][j] = Cell(uvec2(i, j));
					terrain[i][j] = 'G';
				};
		};
		~Map()
		{

		};
		void refresh_grid()
		{
			std::scoped_lock<std::recursive_mutex> lock0{GameObject<Updateable>::mut};
			std::scoped_lock<std::recursive_mutex> lock1{GameObject<Renderable>::mut};

			grid_inds.clear();
			grid_inds.reserve((x_size + 1) * (y_size + 1) * 8);

			for (auto i = 0; i < x_size; i++)
				for (auto j = 0; j < y_size; j++)
					if (terrain[i][j])
					{
						grid_inds.emplace_back((uint32)(i + 0) * (y_size + 1) + (j + 0));
						grid_inds.emplace_back((uint32)(i + 1) * (y_size + 1) + (j + 0));
						grid_inds.emplace_back((uint32)(i + 1) * (y_size + 1) + (j + 0));
						grid_inds.emplace_back((uint32)(i + 1) * (y_size + 1) + (j + 1));
						grid_inds.emplace_back((uint32)(i + 1) * (y_size + 1) + (j + 1));
						grid_inds.emplace_back((uint32)(i + 0) * (y_size + 1) + (j + 1));
						grid_inds.emplace_back((uint32)(i + 0) * (y_size + 1) + (j + 1));
						grid_inds.emplace_back((uint32)(i + 0) * (y_size + 1) + (j + 0));
					};
		};
		void refresh_highlights()
		{
			std::scoped_lock<std::recursive_mutex> lock0{GameObject<Updateable>::mut};
			std::scoped_lock<std::recursive_mutex> lock1{GameObject<Renderable>::mut};
			
			hlht_inds.clear();

			for (auto i = 0; i < x_size; i++)
				for (auto j = 0; j < y_size; j++)
					if (highlights[i][j])
					{
						hlht_inds.emplace_back((uint32)(i + 0) * (y_size + 1) + (j + 0));
						hlht_inds.emplace_back((uint32)(i + 1) * (y_size + 1) + (j + 0));
						hlht_inds.emplace_back((uint32)(i + 1) * (y_size + 1) + (j + 1));
						hlht_inds.emplace_back((uint32)(i + 0) * (y_size + 1) + (j + 1));
					};
		};
		void refresh_heights()
		{
			for (auto i = 0; i < x_size + 1; i++)
				for (auto j = 0; j < y_size + 1; j++)
					vtxs[i * (y_size + 1) + j].z = heights[i][j];
		};
		
		std::vector<vec4>	vtxs;
		std::vector<uint32>	grid_inds;
		std::vector<uint32>	hlht_inds;

		uint8 highlights[x_size][y_size];
		uint8 terrain[x_size][y_size]{};
		float heights[x_size][y_size]{};
		Cell cells[x_size][y_size];
		const char* file_name;

	private:
		void update() override
		{

		};
		void render() override
		{
			glUseProgram(shader.id);

			glEnableVertexAttribArray(VEC4_0_16);

			glUniformMatrix4fv(glGetUniformLocation(shader.id, "mvp"), 1, false, (float*)&Camera::mvp);

			glBufferData(
				GL_ARRAY_BUFFER,
				vtxs.size() * sizeof(vec4),
				vtxs.data(),
				GL_DYNAMIC_DRAW);

			glUniform4fv(glGetUniformLocation(shader.id, "color"), 1, (float*)&vec4(1, 0, 0, 1)[0]);

			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				grid_inds.size() * sizeof(uint32),
				grid_inds.data(),
				GL_DYNAMIC_DRAW);

			glDrawElements(GL_LINES, (GLsizei)grid_inds.size(), GL_UNSIGNED_INT, nullptr);

			glUniform4fv(glGetUniformLocation(shader.id, "color"), 1, (float*)&vec4(0, 0, 1, .5)[0]);

			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				hlht_inds.size() * sizeof(uint32),
				hlht_inds.data(),
				GL_DYNAMIC_DRAW);

			glDrawElements(GL_QUADS, (GLsizei)hlht_inds.size(), GL_UNSIGNED_INT, nullptr);

			glDisableVertexAttribArray(VEC4_0_16);
		};
		static inline Shader shader{"Game/Map"};
	};
};
