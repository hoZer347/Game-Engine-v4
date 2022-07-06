#pragma once

#include "Engine.h"

#include <string>

namespace eng
{
	namespace wnd
	{
		std::unique_ptr<Thread> create(int w, int h, const char* name);

		unsigned int load_shader(Thread* thread, const char* f1);
		unsigned int load_shader(Thread* thread, const char* f1, const char* f2);
		unsigned int load_shader(Thread* thread, const char* f1, const char* f2, const char* f3);

		unsigned int load_texture(Thread* thread, const char* file_name);
	};
};
