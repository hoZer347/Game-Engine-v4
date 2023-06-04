#pragma once

#include "../Data.h"
#include "../Sprite.h"

namespace loom
{
	struct StatSpread
	{
		uint8_t HP, STR, DEX, INT, WIS, DEF, RES, SPD;

		std::string name;
		StatSpread* _next = nullptr;
	};

	struct Sprite;
	
	struct Unit final
	{
		Unit(Sprite& sprite);
		virtual ~Unit();

		Mat4 trns;
		Sprite* sprite = nullptr;
	};
};
