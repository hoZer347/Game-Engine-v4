#include "Engine.h"

#include "Mem_Leak.h"

#include "Object.h"

#include <thread>
#include <vector>

#include <iostream>

struct Engine
{
	static void kernel(Engine* e, unsigned char index)
	{
		while (!e->kill)
			while (e->permissions[index])
			{
				obj::access(e->stage, index);

				e->permissions[index] = false;
			};

		for (auto b : e->permissions)
			b = false;
	};

	static void manager(Engine* e)
	{
		while (!e->kill)
			for (auto& s : e->stages)
			{
				e->stage = s;

				for (auto p : e->permissions)
					p = true;

				for (auto p : e->permissions)
					while (p);
			};
		
		for (auto& t : e->threads)
			t.join();
	};

	std::thread t;
	bool kill = false;
	obj::Stage stage = STG_NULL;

	std::vector<bool> permissions;
	std::vector<obj::Stage> stages;
	std::vector<std::thread> threads;
};

namespace eng
{
	const unsigned char THREAD_COUNT = std::thread::hardware_concurrency();

	Engine* create()
	{
		Engine* e = new Engine();

		return e;
	};

	void start(Engine* e)
	{
		for (unsigned char i = 0; i < std::max(THREAD_COUNT - 1, 1); i++)
		{
			e->permissions.push_back(false);
			e->threads.push_back(std::thread(Engine::kernel, e, i));
		};

		e->t = std::thread(Engine::manager, e);
	};

	void assign_tasks(Engine* e, std::vector<obj::Stage> stages)
	{
		e->stages = stages;
	};

	bool& get_kill(Engine* e)
	{
		return e->kill;
	};

	void wait(Engine* e)
	{
		e->t.join();

		delete e;
	}

	void close(Engine* e)
	{
		e->kill = true;
	};
};
