#pragma once

class Game;

struct Mesh;

class Object
{
public:
	Object();
	virtual ~Object();

	Mesh* m = nullptr;

	Game* game = nullptr;
};

template <class T>
class _Object : public Object
{

};
