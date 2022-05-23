#pragma once

#include "Stages.hpp"

namespace obj
{
	typedef unsigned char Stage;
	
	class Obj
	{
	public:
		Obj(bool=true);

		virtual void exec(Stage&)=0;

		size_t index;
	};

	void access(Stage, unsigned char=0);
	void clear();
};
