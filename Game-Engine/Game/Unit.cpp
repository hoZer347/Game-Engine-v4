#include "Unit.h"

#include "../Data.h"

#include "glm/gtx/transform.hpp"
using namespace glm;

namespace loom
{
	struct UnitManager :
		virtual protected Updateable
	{
		void update() override;

		std::vector<Unit*> units;
	};
	static inline UnitManager manager;


	Unit::Unit(Sprite& sprite) :
		sprite(sprite)
	{
		trns.bind(sprite.trns);
	};
	Unit::~Unit()
	{ };
	void UnitManager::update()
	{ };
};
