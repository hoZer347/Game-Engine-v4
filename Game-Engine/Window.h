#pragma once

#include <memory>

namespace eng
{
	class Thread;
	class Task;

	namespace wnd
	{
		std::unique_ptr<Thread> create(int w, int h, const char* name);

		// Loading Data
		// shader / texture: output
		// thread: thread on which to do the data loading
		// f1->fx: files needed
		void load_shader(unsigned int& shader, Thread* thread, const char* f1);
		void load_shader(unsigned int& shader, Thread* thread, const char* f1, const char* f2);
		void load_shader(unsigned int& shader, Thread* thread, const char* f1, const char* f2, const char* f3);
		void load_texture(unsigned int& texture, Thread* thread, const char* file_name, unsigned int type);
	};
};
