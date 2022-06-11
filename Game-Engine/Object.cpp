#include "Object.h"

#include "Mem_Leak.h"

#include "Game.h"

#include "Mesh.h"

#include "GLEW/glew.h"

Object::Object()
{

};

Object::~Object()
{
	delete m;
};
