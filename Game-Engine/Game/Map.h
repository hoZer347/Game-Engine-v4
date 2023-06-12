#pragma once

#include "../Enums.h"
#include "../Data.h"

#include <glm/glm.hpp>
using namespace glm;

#include <atomic>
#include <vector>

namespace loom
{
	struct alignas(64) Cell final
	{
		const ivec2 pos{ 0 };
		uint8 highlight = 0;
		uint8 filler = 0;

		uint32 v0 = -1, v1 = v0, v2 = v0, v3 = v0;

		Cell	*U = nullptr,
				*D = nullptr,
				*L = nullptr,
				*R = nullptr;
	};

	struct Map final
	{
		Map(const uint8& w, const uint8& h);

		Cell* operator[](auto i) { return &cells.data()[i * h]; }
		const uint8 w, h;

		auto begin() const { return cells.begin(); };
		auto end()   const { return cells.end();   };

	protected:
		friend struct GridOutline;
		friend struct Highlights;

		std::vector<vec4>	vtxs;
		std::vector<Cell>	cells;
		static inline Shader shader{ "Game/Map" };
 	};

	 
	struct GridOutline final :
		virtual protected Loadable,
		virtual protected Renderable
	{
		GridOutline(Map& map);
		void reassign(Map& map);

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
};
