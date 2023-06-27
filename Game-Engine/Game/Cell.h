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
		ptr<Unit> unit;

		uint8 highlight = 0;
		uint8 difficulty = 1;
		
		Cell *U = nullptr, *D = nullptr, *L = nullptr, *R = nullptr;
	};
};
