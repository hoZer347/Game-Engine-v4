#include "Unit.h"

#include "glm/gtx/transform.hpp"
using namespace glm;

namespace loom
{
	Unit::Unit(Sprite& sprite) :
		sprite(sprite),
		trns(sprite.trns)
	{ };
	Unit::~Unit()
	{ };
	void Unit::update()
	{ };
};
