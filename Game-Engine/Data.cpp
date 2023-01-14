#include "Data.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Loom.h"

namespace loom
{
	Object::Object()
	{ 
		objects.push_back(this);
	}
	


	Renderable::Renderable()
	{
		renderables.push_back(this);
	};



	Updatable::Updatable()
	{
		updatables.push_back(this);
	};



	void Helper::assign_on_kill(Task task)
	{
		on_kill.push_back(task);
	};
	void Helper::assign(Task task)
	{
		mut.lock();
		for_kernels.push(task);
		mut.unlock();
	};
	void Helper::push(Task task)
	{
		mut.lock();
		for_tasks.push(task);
		mut.unlock();
	};
	void Helper::load()
	{
		thread = std::thread([this]()
		{
			while (!KILL.load())
			{
				mut.lock();
				while (!for_tasks.empty())
				{
					tasks.push(for_tasks.front());
					for_tasks.pop();
				};
				while (!for_kernels.empty())
				{
					kernels.push_back(for_kernels.front());
					for_kernels.pop();
				};
				mut.unlock();

				for (auto& task : kernels)
					task();

				while (tasks.size())
				{
					tasks.front()();
					tasks.pop();
				};

				while (STOP.load() && !KILL.load());
			};
		});
	};
	void Helper::unload()
	{
		KILL.store(true);
		if (thread.joinable())
			thread.join();
	};
};
