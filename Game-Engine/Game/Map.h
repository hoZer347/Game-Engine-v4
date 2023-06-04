#pragma once

#include "../Enums.h"
#include "../Data.h"

#include <glm/glm.hpp>
using namespace glm;

#include <atomic>
#include <vector>

namespace loom
{
	struct Map final
	{
		Map(const uint16_t& x, const uint16_t& y);

		std::vector<std::vector<float>>& operator[](auto i) { return data[i]; }

		const uint16_t x, y;

	private:
		std::vector<std::vector<std::vector<float>>> data;
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

		std::vector<vec4>		vtxs;
		std::vector<uint32_t>	inds;

		static inline Shader shader{ "Game/Outline" };
	};


	struct Highlights final :
		virtual protected Renderable
	{
		Highlights(Map& map);
		void reassign(Map& map);

	private:
		void render() override;

		std::atomic<std::vector<vec4>*>		vtxs = nullptr;
		std::atomic<std::vector<uint32_t>*>	inds = nullptr;

		static inline Shader shader{ "Game/CellHighlights" };
	};
};
