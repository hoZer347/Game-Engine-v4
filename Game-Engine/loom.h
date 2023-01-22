#pragma once

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

		static const double GetTimeDiff();

		// Add runtime construction function

	private:
		Loom() { };
	};
	//
};
