#include "Data.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Loom.h"

namespace loom
{
	Object::Object()
	{
		if (!Loom::IsRunning())
			objects.push_back(this);
		else Loom::Add(this);
	}
	Object::~Object()
	{ Loom::Rmv(this); };
	


	Renderable::Renderable()
	{
		if (!Loom::IsRunning())
			renderables.push_back(this);
		else Loom::Add(this);
	};
	Renderable::~Renderable()
	{ Loom::Rmv(this); };



	Updatable::Updatable()
	{
		if (!Loom::IsRunning())
			updatables.push_back(this);
		else Loom::Add(this);
	};
	Updatable::~Updatable()
	{ Loom::Rmv(this); };



	Helper::Helper()
	{
		thread = std::thread([this]()
		{
			Task task = []() { };
			
			while (!KILL)
			{
				while (!PASS && !KILL);

				mut.lock();
				if (!in.empty())
				{
					task = in.front();
					in.pop();
				};
				mut.unlock();
				PASS--;

				task();
			};
		});
	};
	Helper::~Helper()
	{
		KILL = true;
		if (thread.joinable())
			thread.join();
	};
	void Helper::assign(Task task)
	{
		mut.lock();
		in.push(task);
		PASS++;
		mut.unlock();
	};
};
