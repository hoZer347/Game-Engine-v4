#pragma once

#include "Data.h"
#include "Sprite.h"

namespace loom
{
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
