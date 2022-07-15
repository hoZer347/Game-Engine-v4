#pragma once

#include "Mem_Leak.h"

#include <memory>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <functional>

// Used to define a task that contain a certain type of object
// NAME: class name of task
// OBJ: type of object inside the task
// ...: additional functionality inside the class (can be left blank)
#define DEFINE_TASK(NAME, DATA, ...)										\
class NAME : public eng::Task												\
{																			\
public:																		\
	static std::unique_ptr<Task> create(DATA* data)							\
	{																		\
		return std::unique_ptr<Task>(new NAME(data));						\
	};																		\
																			\
	void exec() override;													\
																			\
	__VA_ARGS__																\
																			\
	friend class eng::Task;													\
	friend class eng::Thread;												\
	friend class std::unique_ptr<Task>;										\
																			\
protected:																	\
	NAME(DATA* d) { data = d;};												\
	DATA* data;																\
};																			\
																			\
void NAME::exec()															\

// Used to define a thread that executes tasks (you create the first task it executes)
// NAME: class name of thread
// OBJ: type of object inside the thread
// ...: additional functionality inside the class (can be left blank)
#define DEFINE_THREAD(NAME, DATA, ...)										\
class NAME : public eng::Thread												\
{																			\
public:																		\
	static std::unique_ptr<Thread> create(DATA* data)						\
	{																		\
		return std::unique_ptr<Thread>(new NAME(data));						\
	};																		\
																			\
	void exec() override;													\
																			\
	__VA_ARGS__																\
																			\
	friend class eng::Task;													\
	friend class eng::Thread;												\
	friend class std::unique_ptr<Thread>;									\
																			\
protected:																	\
	NAME(DATA* d) { data = d; };											\
	DATA* data;																\
};																			\
																			\
void NAME::exec()															\

namespace eng
{
	// Sets up everything the engine needs
	void init();

	// Gets rid of everything the engine uses by default
	void close();

	// Task that can be performed
	class Task
	{
	public:
		virtual ~Task() { };
		virtual void exec()=0;
	};

	// Thread that executes tasks
	class Thread : public Task
	{
	public:
		Thread() { all_threads.push_back(this); }
		virtual ~Thread() { };
		void init();

		virtual void onInit() { };
		virtual void onKill() { };

		bool idle() { return tasks.empty(); };
		void push(Task*);
		void assign(Task*);
		void clear();
		void join();
		bool joinable() { return t.joinable(); }

		bool KILL	= false;
		bool PAUSE	= false;

		static inline std::vector<Thread*> all_threads;

	protected:
		std::mutex mut1;
		std::mutex mut2;

		std::queue<Task*> tasks;
		std::vector<Task*> kernel;
		std::thread t;
	};

	template <typename T>
	class Object
	{
	public:
		virtual ~Object() { };

		// Creates blank objects
		// num_objs: number of objects to create
		static void create(size_t num_objs = 1)
		{
			data_access.lock();
			data.reserve(num_objs);
			for (size_t i = 0; i < num_objs; i++)
				data.push_back(T());
			data_access.unlock();
		};

		// Access some objects of this class type using a lambda
		// f: lambda used on object(s)
		// begin: the index to start in the data array
		// end: the index to end in the data array (-1 mean only stop at the end)
		static void access(std::function<void(T&)> f = {}, size_t begin = 0, size_t end = -1)
		{
			data_access.lock();
			if (end == -1) end = begin + 1;
			for (size_t i = begin; i < end; i++)
				f(data[i]);
			data_access.unlock();
		};

		static size_t size()			{ return data.size(); };
		static std::mutex& get_mutex()	{ return data_access; };

		friend class std::vector<T>;

	private:
		Object() { };
		static inline std::mutex data_access;
		static inline std::vector<T> data;
	};

	void debug();
};
