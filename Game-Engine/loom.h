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
		static void Construct(std::shared_ptr<S>& s, T&&... args)
		{
			load([&s, args...]() { s = std::make_shared<S>(args...); });
		};

	private:
		static void load(Task task);

		Loom() { };
	};
	//
};
