#include "Data.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include "Loom.h"

namespace loom
{
	Object::Object()
	{
		mut.lock();
		if (!Loom::IsRunning())
			objects.push_back(this);
		else Loom::Add(this);
		mut.unlock();
	}
	Object::~Object()
	{ Loom::Rmv(this); };
	


	Renderable::Renderable()
	{
		mut.lock();
		if (!Loom::IsRunning())
			renderables.push_back(this);
		else Loom::Add(this);
		mut.unlock();
	};
	Renderable::~Renderable()
	{ Loom::Rmv(this); };



	Updatable::Updatable()
	{
		mut.lock();
		if (!Loom::IsRunning())
			updatables.push_back(this);
		else Loom::Add(this);
		mut.unlock();
	};
	Updatable::~Updatable()
	{ Loom::Rmv(this); };
};
