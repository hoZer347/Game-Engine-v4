#pragma once

#include "Data.h"

#include <String>
#include <memory>
#include <vector>
#include <functional>
#include <unordered_set>

#include <glm/glm.hpp>
using namespace glm;

namespace loom
{
	// TODO: add debug system

	// Init / Exit
	void init();
	void exit();
	//


	// Designates a given thread as a window
	void open_window(Thread& thread, int w = 640, int h = 640, const char* name = "");
	//


	//
	void assign(		Thread& thread,		Task tasks);
	void assign_once(	Thread& thread,		Task tasks);
	void assign_on_init(Thread& thread,		Task tasks);
	void assign_on_kill(Thread& thread,		Task tasks);
	//


	//
	void run(Threads threads);
	//


	// TODO: Add string -> void* finding system

	// Loads a Shader using a window thread
	// Guide:
	// Example:
	void load(
		Shader& shader,
		Thread& window,
		std::string name,
		std::vector<std::string> files);
	//


	// Loads a Texture using a window thread
	// Guide:
	// Example:
	void load(
		Texture& texture,
		Thread& window,
		Shader& shader,
		std::string name,
		std::string file,
		TYPE type);
	//


	// Loads a Mesh using a loader thread
	// Guide:
	// Example:
	void load(
		Mesh& mesh,
		Thread& loader,
		Shader& shader,
		Texture& texture,
		DrawMode& draw_mode);
	//
};
