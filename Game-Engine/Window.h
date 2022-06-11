#pragma once

#include "Threadable.h"

class Game;
class Inputs;
class Objects;

struct GLFWwindow;
struct Mesh;

class Window : public Threadable
{
public:
	Window(Inputs*, Objects*, Game*);
	virtual ~Window();
	
	void prex();
	void exec();
	void pstx();

	int width=0, height=0;

	unsigned int
		_vtxs = 0,
		_inds = 0,
		_texs = 0,
		_unfs = 0;

protected:
	Game* game			= nullptr;
	Inputs* inputs		= nullptr;
	Objects* objects	= nullptr;

	GLFWwindow* w		= nullptr;
};
