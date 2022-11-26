#pragma once

#include <memory>
#include <functional>

namespace eng
{
	#define create_task(task) std::make_shared<std::function<bool()>>(task)
	typedef std::shared_ptr<std::function<bool()>> Task;	
};
