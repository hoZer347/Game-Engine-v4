#include "Renderer.h"

#include "Mem_Leak.h"

#include "Window.h"

#include <iostream>

Renderer::Renderer(Window* w)
{
	window = w;
};

Renderer::~Renderer()
{

};

void Renderer::exec()
{
	stop();
};
