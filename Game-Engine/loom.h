#pragma once

#include <functional>

namespace loom
{
	// The main engine singleton
	struct Loom final
	{
		static void Init();
		static void Run();
		static void Exit();

	private:
		Loom() { };
	};
	//
};
