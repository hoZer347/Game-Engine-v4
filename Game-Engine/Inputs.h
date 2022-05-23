#pragma once

#include "Object.h"
#include "Window.h"

class Input;

namespace inputs
{
	// Series of input data
	typedef unsigned int Type;
	
	// Just a regular obj::Obj, but typedef'd for inputs (can be null)
	typedef obj::Obj Obj;

	// A task to execute on an input::Obj
	typedef void(*Task)(Obj*);

	void update();
	void add(Type, Task, Obj);
	void del(Type&);
	void del(Task&);
	void del(Obj&);
	void next();
	void prev();
};
