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
#define DEFINE_TASK(NAME, OBJ, CONTENT)								\
class NAME : public eng::Task										\
{																	\
protected:															\
	NAME(OBJ* obj = nullptr)										\
	{																\
		o = obj;													\
	};																\
																	\
public:																\
	virtual ~NAME() { };											\
																	\
	OBJ* GetObject()												\
	{																\
		return o;													\
	};																\
																	\
	static std::unique_ptr<Task> create(OBJ* o)						\
	{																\
		return std::unique_ptr<Task>(new NAME(o));					\
	};																\
																	\
	void exec() override;											\
																	\
	CONTENT															\
																	\
	friend class std::unique_ptr<Task>;								\
																	\
protected:															\
	OBJ* o;															\
};																	\
																	\
void NAME::exec()													\

// Used to define a thread that executes tasks (you create the first task it executes)
// NAME: class name of thread
// OBJ: type of object inside the thread
// CONTENT: additional functionality inside the class (can be left blank)
#define DEFINE_THREAD(NAME, OBJ, CONTENT)							\
class NAME : public eng::Task, public eng::Thread					\
{																	\
protected:															\
	NAME(OBJ* obj = nullptr)										\
	{																\
		o = obj;													\
		assign(this);												\
	};																\
																	\
public:																\
	virtual ~NAME() { };											\
																	\
	OBJ* GetObject()												\
	{																\
		return o;													\
	};																\
																	\
	static std::unique_ptr<Thread> create(OBJ* o)					\
	{																\
		return std::unique_ptr<Thread>(new NAME(o));				\
	};																\
																	\
	void exec() override;											\
																	\
	CONTENT															\
																	\
	friend class std::unique_ptr<Thread>;							\
																	\
protected:															\
	OBJ* o;															\
};																	\
																	\
void NAME::exec()													\

#define DEFINE_OBJECT(NAME, TYPE, CONTENT)

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
	class Thread
	{
	public:
		virtual ~Thread() { };
		virtual void init();

		void assign(Task*);
		void join();

		std::mutex mut;

		bool KILL	= false;
		bool PAUSE	= false;

		friend class std::thread;

	protected:
		std::queue<Task*> tasks;
		std::thread t;
	};

	// Object for storing / manipulating data
	class Object
	{
	public:
		virtual ~Object() { };
	};
};
