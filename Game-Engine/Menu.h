#pragma once

#include <vector>

namespace eng
{
	struct Thread;

	struct Menu
	{
		Menu(Thread*);
		~Menu();

		struct Option
		{
			virtual void on()		{ };
			virtual void off()		{ };
			virtual bool collide()	{ return false; };
		};

		virtual void abort();

		Option* selected = nullptr;
		std::vector<Option*> options;
	};
};
