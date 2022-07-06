#pragma once

#include "Engine.h"

#include "glm/glm.hpp"
using namespace glm;

#include <vector>
#include <map>

namespace eng
{
	namespace spr
	{
		struct SpriteData
		{
			std::vector<float> vtxs;
			std::vector<unsigned int> inds;
			unsigned int shader = 0, tex = 0, atb1 = 0, atb2 = 0;
		};

		SpriteData* create(Thread* thread);
	};
};
