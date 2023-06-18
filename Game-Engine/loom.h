#pragma once

#include <functional>
#include <memory>

namespace loom
{
	typedef std::function<void()> Task;

	// The main engine singleton
	struct Engine final
	{
		static void Init();
		static void Run();
		static void Exit();

		static void Exec(Task&& task);
		static const bool& GetIsRunning();

		template <typename S>
		static inline std::shared_ptr<S> make(auto&&... args) { return std::shared_ptr<S>(new S(args...)); };

	private:
		Engine() { };
	};
	//
};
