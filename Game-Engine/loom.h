#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include <string>

namespace loom
{
	struct Loom
	{
		static void Init();
		static void RunOnThisThread();
		static void Exit();
	};
};
