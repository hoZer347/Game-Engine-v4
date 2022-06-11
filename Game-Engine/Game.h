#pragma once

#include "Threadable.h"

class Inputs;
class Objects;
class Renderer;
class Window;

struct Mesh;

class Game : public Threadable
{
public:
	Game();
	virtual ~Game();
	void init();
	void end();

	void exec();
	void pstx();

//protected:
	Inputs* inputs		= nullptr;
	Objects* objects	= nullptr;
	Renderer* renderer	= nullptr;
	Window* window		= nullptr;
};
