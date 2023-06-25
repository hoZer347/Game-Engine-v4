#pragma once

#include "../Data.h"
#include "Unit.h"

namespace loom
{
	enum CELL
	{
		GRASS,
		WATER,
	};


	struct Cell final
	{
		Unit* unit = nullptr;
		Cell *U = nullptr, *D = nullptr, *L = nullptr, *R = nullptr;
	};
};
