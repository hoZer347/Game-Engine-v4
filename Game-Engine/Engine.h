#pragma once

#include "Task.h"

#include <memory>
#include <string>
#include <unordered_set>

namespace loom
{
	struct Thread;
	struct Barrier;
	struct Renderable;
	template <typename T> struct Buffer;


	//
	void init();


	//
	void open_window(Thread* thread, const char* name = "", int w = 640, int h = 640);


	//
	void assign			(Task task);
	void assign			(Thread* thread, Task task);
	void assign_once	(Thread* thread, Task task);
	void assign_on_init	(Thread* thread, Task task);
	void assign_on_kill	(Thread* thread, Task task);

	_NODISCARD Task _render_func	(Buffer<Renderable>& buffer) noexcept;
	_NODISCARD Task _load_shader	(Renderable& renderable, std::string file_name) noexcept;
	_NODISCARD Task _load_shader	(Renderable& renderable, std::vector<std::string> file_names) noexcept;
	_NODISCARD Task _load_textures	(Renderable& renderable, std::vector<std::pair<std::string, unsigned int>> texture_files) noexcept;


	//
	void sync(std::unordered_set<Thread*> threads,
			  std::shared_ptr<Barrier> barrier,
			  Task on_cycle = []() noexcept { });


	//
	void run();
	void run(std::unordered_set<Thread*> threads);


	//
	void close();
	void close(std::unordered_set<Thread*> threads);


	//
	Thread* create_thread();
	void destroy(Thread* thread);
};
