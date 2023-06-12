#pragma once

#include "../Enums.h"
#include "../Data.h"
#include "../Geometry.h"

#include <glm/glm.hpp>
using namespace glm;

#include <atomic>
#include <vector>

namespace loom
{
	struct Cell;

	struct Map final :
		virtual protected Updateable
	{
		Map(const uint8& w, const uint8& h);

		Cell* operator[](auto i) { return &cells.data()[i * h]; }
		const uint8 w, h;

		auto begin() const { return cells.begin(); };
		auto end()   const { return cells.end();   };

	protected:
		friend struct GridOutline;
		friend struct Highlights;

		Square hovered;
		std::vector<vec4>	vtxs;
		std::vector<Cell>	cells;
		static inline Shader shader{ "Game/Map" };

	private:
		void update() override;
 	};

	 
	struct GridOutline final :
		virtual protected Loadable,
		virtual protected Renderable
	{
		GridOutline(Map& map);
		void reassign(Map& map);

		bool show = true;

	private:
		void load() override;
		void render() override;

		Map& map;
		std::vector<uint32>	inds;
	};


	struct Highlights final :
		virtual protected Loadable,
		virtual protected Renderable
	{
		Highlights(Map& map);
		void reassign(vec4&& new_color = vec4{1});

	private:
		void load() override;
		void render() override;

		Map& map;
		vec4 color;
		std::atomic<std::shared_ptr<std::vector<uint32>>>	inds = nullptr;
		std::atomic<std::shared_ptr<std::vector<uint32>>>	outs = nullptr;
	};


	struct Hovered final
	{
		static inline Cell* cell = nullptr;
		static inline std::vector<uint32> inds;
	};
};
