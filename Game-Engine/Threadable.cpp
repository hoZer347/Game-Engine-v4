#include "Threadable.h"

#include "Mem_Leak.h"

#include <iostream>

Threadable::Threadable()
{

};

Threadable::~Threadable()
{
	
};

void Threadable::init()
{
	thread = std::thread([this]()
		{
			prex();

			while (!KILL) {
				exec();

				while (PAUSE) if (KILL) break;
			};

			pstx();
		});
};

void Threadable::start()
{
	PAUSE = false;
};

void Threadable::stop()
{
	PAUSE = true;
};

void Threadable::kill()
{
	PAUSE = false;
	KILL = true;
	PAUSE = false;

	thread.join();
};

void Threadable::wait()
{
	thread.join();
};
