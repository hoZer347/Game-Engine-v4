#pragma once

#include "../Data.h"

namespace loom
{
	struct Unit;
	struct Cell final
	{
		ptr<Unit> unit;

		vec4* pos = nullptr;

		uint8 highlight = 0;
		uint8 difficulty = 1;
		
		Cell *U = nullptr, *D = nullptr, *L = nullptr, *R = nullptr;
	};
};
