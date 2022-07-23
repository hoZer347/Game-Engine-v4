#pragma once

#include "Thread.h"
#include "Mesh.h"

#include <iostream>

namespace eng
{
	struct Renderer {

		template<ThreadID ID>
		static void open_window(const char* title="", int width=640, int height=640)
		{
			Thread<ID>::push([title, width, height]()
				{
					setup(title, width, height);
					Thread<ID>::assign(update);
				});
		};

		template <ThreadID ID>
		static void render_this(Material& mat, std::vector<Mesh>& m)
		{
			Thread<ID>::assign([&mat, &m]()
				{
					render_this(mat, m);
				});
		};

		template <ThreadID ID>
		static void load_texture(Material& mat, const char* file_name, unsigned int type)
		{
			Thread<ID>::push([&mat, file_name, type]()
				{
					mat.texs.push_back(load_texture(file_name, type));
				});
		};

		template <ThreadID ID>
		static void load_shader(Material& mat, const char* f1)
		{
			Thread<ID>::push([&mat, f1]()
				{
					mat.shader = load_shader(f1);
				});
		};

		template <ThreadID ID>
		static void load_shader(Material& mat, const char* f1, const char* f2)
		{
			Thread<ID>::push([&mat, f1, f2]()
				{
					mat.shader = load_shader(f1, f2);
				});
		};

		template <ThreadID ID>
		static void load_shader(Material& mat, const char* f1, const char* f2, const char* f3)
		{
			Thread<ID>::push([&mat, f1, f2, f3]()
				{
					mat.shader = load_shader(f1, f2, f3);
				});
		};

	private:
		static bool test_for_window();
		static void setup(const char* title, int width, int height);
		static void update();
		static unsigned int load_shader (const char* f1);
		static unsigned int load_shader (const char* f1, const char* f2);
		static unsigned int load_shader (const char* f1, const char* f2, const char* f3);
		static unsigned int load_texture(const char* file_name, unsigned int type);
		static void render_this(Material& mat, std::vector<Mesh>& m);
	};

	// Interact With the Text Rendering Engine
	struct Text
	{
		template <ThreadID ID>
		static void init()
		{
			Thread<ID>::push(_init);
		};

		template <ThreadID ID>
		static void load_font(Material& mat, const char* file_name)
		{
			Thread<ID>::push([&mat, file_name]()
				{
					load_font(mat, file_name);
				});
		};

	private:
		static void load_font(Material& mat, const char* file_name);
		static void _init();
	};

	// Interact With the Input Engine
	struct Input
	{
		template <ThreadID ID>
		static void init()
		{
			Thread<ID>::push(_init);
		};

	private:
		static void _init();
	};

	// Interact with the Sound Engine
	struct Sound
	{
		template <ThreadID ID>
		static void init()
		{
			Thread<ID>::push(_init);
		};

	private:
		static void _init();
	};
};
