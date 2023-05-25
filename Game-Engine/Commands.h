#pragma once

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	struct Commands
	{
		// Modifies the current input level to allow free camera movement
		static void InitiateFreeCam();

	private:
		Commands() { };
	};
};
