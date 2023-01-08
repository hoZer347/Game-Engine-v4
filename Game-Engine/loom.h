#pragma once

namespace loom
{
	void Init() noexcept;
	void Run() noexcept;
	void Exit() noexcept;

	struct Mesh;
	struct Window;

	typedef int RESULT;

	RESULT load(Window& window, Mesh& mesh);
	RESULT remv(Window& window, Mesh& mesh);
	RESULT open(Window& window, int width, int height, const char* title);
};
