#pragma once

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	struct Grid;

	struct Commands
	{
		static void InitiateFreeCam();

	private:
		Commands() { };
	};
};
