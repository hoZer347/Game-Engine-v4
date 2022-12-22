#pragma once

#include "Task.h"
#include "Data.h"

#include "glm/glm.hpp"
using namespace glm;

namespace loom
{
	struct Menu;
	struct MenuItem;
	struct Thread;

	struct MenuItem
	{
		Task& update	= null;
		Task& handle	= null;

		vec3 pos;
		vec3 size;
	};
	
	struct Option : public MenuItem
	{
		Task& on_hover = null;
		Task& on_press = null;
	};

	struct Menu final
	{
		Menu(Thread* thread, Thread* window = nullptr);

		void add(std::vector<MenuItem*> items);

	protected:
		friend struct MenuItem;
		_NODISCARD Task _update();
		_NODISCARD Task _handle();
		std::atomic<double> xpos = 0, ypos = 0;
		std::vector<MenuItem*> items;

	private:
		Menu() { };
	};
};
