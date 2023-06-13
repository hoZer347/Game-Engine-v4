#pragma once

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	struct Unit;

	struct Cell final
	{
		const ivec2 pos{ 0 };
		uint32 highlight = 0;

		uint32 v0 = -1, v1 = v0, v2 = v0, v3 = v0;

		Unit* unit = nullptr;
		Cell* U = nullptr,
			* D = nullptr,
			* L = nullptr,
			* R = nullptr;
	};
};
