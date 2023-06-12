#pragma once

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	struct alignas(64) Cell final
	{
		vec3 pos{ 0 };
		uint32 highlight = 0;

		uint32 v0 = -1, v1 = v0, v2 = v0, v3 = v0;

		Cell* U = nullptr,
			* D = nullptr,
			* L = nullptr,
			* R = nullptr;
	};
};
