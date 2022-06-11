#pragma once

#include "Object.h"

#include <vector>

class Objects
{
public:
	Objects();
	virtual ~Objects();

	void get(void(*)(Object*), size_t=0);
	void add(Mesh*);

protected:
	std::vector<Object*> objects;
};
