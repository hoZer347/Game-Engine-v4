#pragma once

#include "Object.h"

#include <vector>

struct Engine;

namespace eng
{
	Engine* create();

	void start(Engine*);

	void assign_tasks(Engine*, std::vector<obj::Stage>);

	bool& get_kill(Engine*);

	void wait(Engine*);

	void close(Engine*);

	extern const unsigned char THREAD_COUNT;
};
