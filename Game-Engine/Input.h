#pragma once

namespace eng {
	class Thread;
	class Task;

#include <memory>

namespace eng {
	namespace inp
	{
		// Makes inputs execute on a thread
		void assign(Thread*);

		// Bind an input to a task
		void bind(unsigned int type, Task* task);
		
		// Creates a new inputs structure ontop of the old one
		void next();

		// Goes back to the old inputs structure, deleting the current one
		void prev();
	};
};
