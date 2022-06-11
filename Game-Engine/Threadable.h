#pragma once

#include <thread>

class Threadable
{
public:
	Threadable();
	virtual ~Threadable();

	void init();

	virtual void prex() { };
	virtual void exec()=0;
	virtual void pstx() { };

	void start();
	void stop();
	void kill();
	void wait();

	friend class Game;

protected:
	bool KILL = false;
	bool PAUSE = false;
	std::thread thread;
};

template <unsigned long long interval = 1000>
class Timed_Threadable : Threadable
{
	// TODO
};
