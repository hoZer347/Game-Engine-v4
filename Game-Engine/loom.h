#pragma once

#include <functional>
#include <memory>

namespace loom
{
	typedef std::function<void()> Task;

	// The main engine singleton
	struct Loom final
	{
		static void Init();
		static void Run();
		static void Exit();

		static void Exec(Task&& task);
		static const bool& GetIsRunning();

	private:
		Loom() { };
	};
	//
};
