#include "Objects.h"

#include "Mem_Leak.h"

#include "Objects.h"
#include "Inputs.h"
#include "Mesh.h"

Objects::Objects()
{

};

Objects::~Objects()
{

};

void Objects::get(void(*f)(Object*), size_t index)
{
	for (auto& o : objects)
		f(o);
};

void Objects::add(Mesh* m)
{
	Object* o = new Object();
	o->m = m;

	objects.push_back(o);
};
