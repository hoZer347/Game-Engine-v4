#pragma once

class Window;
struct Engine;

namespace win
{
	Window* create(Engine*);

	void del(Window*);
};
