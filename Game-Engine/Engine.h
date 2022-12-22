#pragma once

#include "Task.h"

#include <memory>
#include <string>
#include <unordered_set>

namespace loom
{
	struct Thread;
	template <typename T> struct Buffer;


	//
	void init();
	void finish();


	//
	void open_window	(Thread* thread, const char* name = "", int w = 640, int h = 640);


	//
	void assign			(Thread* thread, Task task);
	void assign_once	(Thread* thread, Task task);
	void assign_on_init	(Thread* thread, Task task);
	void assign_on_kill	(Thread* thread, Task task);


	//
	void run();
	void run(std::unordered_set<Thread*> threads);


	//
	void close();
	void close(std::unordered_set<Thread*> threads);


	//
	Thread* create_thread();
	void destroy(Thread* thread);
};
