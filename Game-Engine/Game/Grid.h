#pragma once

#include <glm/glm.hpp>
using namespace glm;


#include "../Data.h"


#include <unordered_set>
#include <functional>
#include <memory>


namespace loom
{
	typedef std::function<void()> Task;
	
	struct Unit;
	struct Terrain;

	struct Cell
	{
		const ivec2 pos;
		int32_t v0 = -1, v1 = v0, v2 = v0, v3 = v0;
		Task onHover = []() {};
		Task onUnhover = []() {};
		Cell* U = nullptr, * D = nullptr, * L = nullptr, * R = nullptr;
		Unit* unit;
		Terrain* terrain;
	};

	struct Grid final :
		virtual private Loadable,
		virtual private Updateable,
		virtual private Renderable
	{
	protected:
		friend struct Commands;

		struct Column
		{
			vec4* ptr;
			vec4& operator[](uint16_t i) { return ptr[i]; }
		};

		struct Hovered
		{
			static inline Cell* cell = nullptr;
			static inline std::vector<uint32_t> inds{ 0, 1, 2, 3 };
		};

		std::vector<vec4> vtxs;
		std::vector<uint32_t> inds;

		std::atomic<std::vector<uint32_t>*> reds = nullptr;
		std::atomic<std::vector<uint32_t>*> blues = nullptr;

		size_t x_size, y_size;

		std::vector<std::vector<Cell*>> cells;

	public:
		Grid(size_t x_size, size_t y_size);
		virtual ~Grid();

		void addUnit(Unit* unit, uint32_t x, uint32_t y);
		auto& operator[](auto i) { return cells[i]; };

	protected:
		friend struct Utils;
		void load() override;
		void update() override;
		void render() override;
	};
};
