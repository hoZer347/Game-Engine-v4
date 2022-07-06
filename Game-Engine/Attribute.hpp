#pragma once

#include <vector>

#include <atomic>

namespace eng
{
	class AttributeManager
	{
		static inline std::atomic<unsigned int> ID=0;

	public:
		static unsigned int create()
		{
			return ID++;
		};
	};
}
