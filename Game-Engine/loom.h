#pragma once

#include <functional>

namespace loom
{
	typedef std::function<void()> Task;

	// The main engine singleton
	struct Loom final
	{
		static void Init();
		static void RunOnThisThread();
		static void Exit();

		// Hands off the construction of a given item to the loader thread
		template <typename S, typename... T>
		static void Construct(std::unique_ptr<S>& s, T&&... args)
		{
			assign([&s, args...]() { s = std::make_unique<S>(args...); });
		};
		//

	private:
		static void assign(Task task);

		Loom() { };
	};
	//
};
