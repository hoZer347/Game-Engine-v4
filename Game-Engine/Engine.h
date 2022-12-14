#pragma once

#include "Task.h"

#include <vector>

namespace loom
{
	struct Thread;

	//
	void init();


	//
	void open_window(Thread* thread, const char* name = "", int w = 640, int h = 640);


	//
	void assign		   (Thread* thread, Task task);
	void assign_on_init(Thread* thread, Task task);
	void assign_on_kill(Thread* thread, Task task);


	//
	void run(std::vector<Thread*> threads);


	//
	void close();


	//
	Thread* create_thread();
};
