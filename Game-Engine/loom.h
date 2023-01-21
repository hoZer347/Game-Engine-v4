#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include <string>

namespace loom
{
	struct Object;
	struct Renderable;
	struct Updatable;

	// The main engine singleton
	struct Loom final
	{
		static void Init();
		static void RunOnThisThread();
		static void Exit();

		static uint32_t GetTimeDiff();

	private:
		Loom() { };
	};
	//
};
