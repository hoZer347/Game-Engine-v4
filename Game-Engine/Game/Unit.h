#pragma once

#include "../Camera.h"
#include "../Sprite.h"

namespace loom
{
	struct alignas(8) StatSpread final
	{
		uint8
			HP	= 0,
			STR = 0,
			DEX = 0,
			INT = 0,
			WIS = 0,
			DEF = 0,
			RES = 0,
			MOV = 0;
	};
	struct Unit final :
		public GameObject<Unit>
	{
		Unit(Sprite& sprite) :
			sprite(sprite)
		{ };

		
		Sprite& sprite;
	};
};
