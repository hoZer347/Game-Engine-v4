#pragma once

#include "Task.h"
#include "Data.h"

namespace loom
{
	struct Window
	{
		virtual ~Window() { };
		virtual void assign(Task task)=0;
		virtual void join()=0;
		virtual bool joinable() const noexcept=0;

		static Window* create(const char* name = "", int w = 640, int h = 640);

	protected:
		friend struct Renderable;
		virtual void load_shader(unsigned int& i, std::vector<std::string> file_names)=0;
		virtual void load_textures(std::vector<unsigned int>& i, std::vector<std::pair<std::string, unsigned int>> textures)=0;
		Window() { };
	};
};
