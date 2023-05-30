#pragma once

#include "../Data.h"
#include "../Sprite.h"

namespace loom
{
	struct StatSpread
	{
		uint8_t
			HP,
			STR,
			DEX,
			INT,
			WIS,
			DEF,
			RES,
			SPD;

		std::string name;
		StatSpread* _next = nullptr;
	};

	struct Unit final :
		virtual protected Updateable
	{
		Unit(Sprite& sprite);
		virtual ~Unit();

		void update() override;

		mat4& trns;
		
	private:
		Sprite& sprite;
	};
};
