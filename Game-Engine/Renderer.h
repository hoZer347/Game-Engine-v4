#pragma once

#include "Threadable.h"

class Window;

class Renderer : public Threadable
{
public:
	Renderer(Window*);
	virtual ~Renderer();

	void exec();

protected:
	Window* window = nullptr;
};

