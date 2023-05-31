#pragma once

#include "Data.h"

namespace loom
{
	struct CollisionGroup final
	{
		CollisionGroup(std::vector<_Geometry*>&& objs);

		void push(std::vector<_Geometry*>&& objs);

	protected:
		friend struct CollisionManager;

		void do_collision();
		static void collide(_Geometry* g0, _Geometry* g1);

		std::vector<_Geometry*> objs;
	};
};
