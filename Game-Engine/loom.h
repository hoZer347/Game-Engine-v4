#pragma once

namespace loom
{
	// The main engine singleton
	struct Loom final
	{
		static void Init();
		static void RunOnThisThread();
		static void Exit();

	private:
		Loom() { };
	};
	//
};
