#pragma once

#include <functional>
#include <memory>

namespace loom
{
	typedef std::function<void()> Task;

	template <typename T> struct ptr;

	// The main engine singleton
	struct Engine final
	{
		// Sets up the engine to be ran later
		static void Init();

		// Runs the engine using the current thread as an openGL host
		static void Run();

		// Runs a thread with the given task, guarantees that it won't add another of the same type at the time
		static void Exec(Task&& task);

		// Returns whether or not the program is running
		static const bool& GetIsRunning();

		// Adds the object(s) in the parameters to the engine
		template <typename T, typename... ARGS>
		static inline void Add(T* t, ARGS... to_add);

		// Removes the object(s) in the parameters to the engine
		template <typename T, typename... ARGS>
		static inline void Rmv(T* t, ARGS... to_add);

	private:
		static inline void Add();
		static inline void Rmv();
		Engine() { };
	};
	//
};
