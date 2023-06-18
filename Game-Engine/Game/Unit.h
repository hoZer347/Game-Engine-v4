#pragma once

#include "../Data.h"

namespace loom
{
	struct StatSpread
	{
		uint8 HP, STR, DEX, INT, WIS, DEF, RES, SPD;

		std::string name;
		StatSpread* _next = nullptr;
	};
	
	struct Cell;
	struct Sprite;
	struct Animation;

	struct Unit :
		public GameObject<Unit>
	{
		Unit(Sprite& sprite, Cell& cell);
		virtual ~Unit();

		StatSpread stats;
		Cell& cell;
		Sprite& sprite;
		Animation* animation = nullptr;
	};
};
