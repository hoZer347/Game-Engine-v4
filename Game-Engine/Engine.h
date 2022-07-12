#pragma once

#include "Mem_Leak.h"

#include <memory>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>

// Used to define a task that contain a certain type of object
// NAME: class name of task
// OBJ: type of object inside the task
// CONTENT: additional functionality inside the class (can be left blank)
#define DEFINE_TASK(NAME, DATA, CONTENT)									\
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
	CONTENT																	\
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
// CONTENT: additional functionality inside the class (can be left blank)
#define DEFINE_THREAD(NAME, DATA, CONTENT)									\
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
	CONTENT																	\
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

// Starts a class definition the inherits from eng::Object
// NAME: class name of object
#define DEFINE_OBJECT(NAME)													\
struct NAME : public eng::Object<NAME>										\

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
	struct Object
	{
	public:
		static std::vector<T>& data()
		{
			return objs;
		};

		static void lock()
		{
			data_access.lock();
		};

		static void unlock()
		{
			data_access.unlock();
		};

		static void create(unsigned int num_objs=1)
		{
			data_access.lock();

			for (int i = 0; i < num_objs; i++)
				objs.push_back(T());
			
			data_access.unlock();
		};

	protected:
		static inline std::mutex data_access;
		static inline std::vector<T> objs;
	};
};
