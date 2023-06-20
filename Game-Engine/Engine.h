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

		// Runs a thread with the given task, guarantees that it won't add another of the same type at the time
		static void Exec(Task&& task);

		// Returns whether or not the program is running
		static const bool& GetIsRunning();

		// Returns whether or not the current thread is the main (render) thread
		// TODO: Implement this
		static const bool& GetIsOnMain();

		// Returns whether or not the current thread is the updater thread
		// TODO: Implement this
		static const bool& GetIsOnUpdater();

		// Adds the object(s) in the parameters to the engine
		template <typename T, typename... ARGS>
		static void Add(T* t, ARGS... to_add);

		// Removes the object(s) in the parameters to the engine
		template <typename T, typename... ARGS>
		static void Rmv(T* t, ARGS... to_add);

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


	// Guarantees memory safe lifetime management
	// Works like a std::shared_ptr
	template <typename T>
	struct ptr : std::shared_ptr<T>
	{
		template <typename... ARGS>
		ptr(ARGS... args) : std::shared_ptr<T>(new T(args...))
		{
			Engine::Add(std::shared_ptr<T>::get());
		};
		ptr(T* t) : std::shared_ptr<T>(t)
		{
			Engine::Add(std::shared_ptr<T>::get());
		};
		virtual ~ptr()
		{
			Engine::Rmv(std::shared_ptr<T>::get());
		};
	};
};
