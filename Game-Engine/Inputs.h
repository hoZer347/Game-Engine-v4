#pragma once

#include <atomic>

class Inputs
{
public:
	Inputs();
	virtual ~Inputs();

	void update();
	void next();
	void prev();

protected:
	Inputs
		*_prev=nullptr,
		*_next=nullptr;
};
