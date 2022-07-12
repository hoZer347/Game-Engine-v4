#pragma once

#include "Attribute.hpp"

#include "Engine.h"

#include <vector>
#include <map>

namespace eng
{
	DEFINE_OBJECT(SpriteData)
	{
		std::vector<float> vtxs =
			{
				-1, -1,  0,  0,  0,
				 1, -1,  0,  1,  0,	
				 1,  1,  0,  1,  1,
				-1,  1,  0,  0,  1,
			};

		std::vector<unsigned int> inds = { 0, 1, 2, 3 };
		unsigned int tex = 0;

		static inline unsigned int
			shader = 0,
			atb1 = AttributeManager::create(),
			atb2 = AttributeManager::create();

		static void init(Thread* thread);
	};
};
