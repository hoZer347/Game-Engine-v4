#pragma once

#include "Engine.h"

#include "glm/glm.hpp"
using namespace glm;

#include "Attribute.hpp"

#include <vector>
#include <map>

namespace eng
{
	namespace spr
	{
		struct SpriteData
		{
			std::vector<float> vtxs =
				{
					-1, -1,  0,  0,  0,
					 1, -1,  0,  1,  0,	
					 1,  1,  0,  1,  1,
					-1,  1,  0,  0,  1
				};

			std::vector<unsigned int> inds = { 0, 1, 2, 3 };
			unsigned int shader = 0, tex = 0;

			static inline unsigned int
				atb1 = AttributeManager::create(),
				atb2 = AttributeManager::create();
		};

		SpriteData* create(Thread* thread);
	};
};
