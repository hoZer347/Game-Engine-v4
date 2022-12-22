#pragma once

#include <functional>

namespace loom
{
	typedef std::function<void()> Task;

	static Task null = []() { };
};
