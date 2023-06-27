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
		virtual public Updateable
	{
		vec4 pos{ 0, 0, 0, 1 };
		ptr<Sprite> sprite;

	protected:
		friend struct ptr<Unit>;
		Unit(ptr<Sprite> sprite) :
			sprite(sprite)
		{ };

	private:
		void update() override
		{
			mat4 _trns = translate(vec3(pos) + vec3(.5, .5, 0));
			_trns *= Camera::yaww_mat;
			_trns *= translate(vec3(-.5, 0, 0));

			sprite->trns = _trns;
		};
	};
};
