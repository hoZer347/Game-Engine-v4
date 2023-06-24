#pragma once

#include <memory>
#include <functional>

namespace loom
{
	typedef std::function<void()> Task;

	// The main engine singleton
	struct Engine final
	{
		// Sets up the engine to be ran later
		static void Init();

		// Runs the engine using the current thread as an openGL host
		static void Run();

		// Returns whether or not the program is running
		static const bool& GetIsRunning();

		// Returns whether or not the current thread is the main (render) thread
		// TODO: Implement this
		static const bool& GetIsOnMain();

		// Returns whether or not the current thread is the updater thread
		// TODO: Implement this
		static const bool& GetIsOnUpdater();

		// Adds the object(s) in the parameters to the engine
		template <typename ARG, typename... ARGS>
		static void Add(ARG* arg, ARGS&&... args);

		// Removes the object(s) in the parameters to the engine
		template <typename ARG, typename... ARGS>
		static void Rmv(ARG* arg, ARGS&&... args);

		// Sends a task to be done by the Main thread (Renderer)
		// If ran on main thread, will handle other functions called here
		static void DoOnMain(Task&& task);

		// Sends a task to be done by the Updater thread
		// If ran on updater, will handle other functions called here
		static void DoOnUpdater(Task&& task);

	private:
		static inline void Add();
		static inline void Rmv();
		Engine() { };
	};
	//
};
