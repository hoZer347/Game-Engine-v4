#include "Utils.h"

namespace loom
{
	double Utils::TimedFunc(void(*f)())
	{
		Timer TIMER;
		TIMER.restart();
		f();
		return TIMER.GetDiff_mls();
	}
};
