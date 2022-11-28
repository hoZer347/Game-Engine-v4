#pragma once

#include "Data.h"

#include <memory>
#include <functional>

namespace eng
{
	struct Task
	{
		Task(auto task)
		{ this->task = std::make_shared<std::function<bool()>>(task); };

		inline bool operator()()	{ return (*task)();  };
		inline operator bool()		{ return (bool)task; };

	private:
		std::shared_ptr<std::function<bool()>> task;
	};
};
