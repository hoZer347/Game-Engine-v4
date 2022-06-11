#include "Game.h"

#include "Mem_Leak.h"

#include "Inputs.h"
#include "Mesh.h"
#include "Objects.h"
#include "Renderer.h"
#include "Window.h"

#include <iostream>

Game::Game()
{
	inputs =  new Inputs();
	objects = new Objects();
	window =  new Window(inputs, objects, this);
	renderer = new Renderer(window);
};

Game::~Game()
{

};

void Game::init()
{
	Threadable::init();

	renderer->init();
	window->init();
};

void Game::end()
{
	KILL = true;
};

void Game::exec()
{
	renderer->start();
	window->start();

	while (!renderer->PAUSE || !window->PAUSE) if (KILL) break;
};

void Game::pstx()
{
	renderer->kill();
	delete renderer;

	window->kill();
	delete window;

	delete inputs;
	delete objects;
};
