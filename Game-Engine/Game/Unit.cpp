#include "Unit.h"

#include "../Data.h"
#include "../Sprite.h"
#include "../Camera.h"
#include "Map.h"
#include "Cell.h"

#include "glm/gtx/transform.hpp"
using namespace glm;

namespace loom
{
	struct UnitManager :
		virtual protected Updateable
	{
		void update() override;
	};
	static inline UnitManager manager;


	Unit::Unit(Sprite& sprite, Cell& cell) :
		sprite(sprite),
		cell(cell)
	{
		cell.unit = this;
	};
	Unit::~Unit()
	{ };
	void UnitManager::update()
	{
		GameObject<Unit>::access([](auto& unit)
		{
			// Resetting Position
			mat4 mat{ 1 };


			// If no animation is present, keep sprite in place on cell
			if (!unit.animation)
				mat *= translate(vec3((vec2)unit.cell.pos + vec2(0, .5), 0));


			// Facing Camera
			mat4 rotation{ 1 };
			rotation *= translate(vec3(.5, 0, 0));
			rotation *= Camera::yaww_mat;
			rotation *= translate(vec3(-.5, 0, 0));
			mat *= rotation;


			// Setting new sprite position
			unit.sprite.trns = mat;
		});
	};
};
