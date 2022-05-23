#include "Object.h"

#include "Mem_Leak.h"

#include "Engine.h"

#include <vector>

namespace obj
{
	std::vector<Obj*> OBJS;

	Obj::Obj(bool should_add)
	{
		index = OBJS.size();

		if (should_add)
			OBJS.push_back(this);
	};

	void access(Stage s, unsigned char start)
	{
		for (size_t i = start; i < OBJS.size(); i += eng::THREAD_COUNT)
			OBJS[i]->exec(s);
	};
};
