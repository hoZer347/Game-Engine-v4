#pragma once

#include <String>
#include <memory>
#include <vector>
#include <thread>
#include <functional>
#include <unordered_set>

#include <glm/glm.hpp>
using namespace glm;

#ifndef MAX_THREADS
#define MAX_THREADS 256
#endif

namespace loom
{
	// Enumeration Types
	typedef uint32_t ID;
	typedef uint32_t TYPE;
	typedef uint32_t Ind;
	//

	// Object Types
	typedef ID Shader;
	typedef ID Texture;
	typedef ID DrawMode;

	typedef std::function<void()>	Task;
	typedef std::vector<Task>		Tasks;

	struct Buffer;
	struct Thread;
	typedef std::vector<Thread*> Threads;

	struct alignas(64) Vtx final
	{
		vec4 pos;
		vec4 clr;
		vec4 nrm;
		vec4 cdr;
	};

	typedef std::vector<Vtx> Vtxs;
	typedef std::vector<Ind> Inds;

	struct alignas(128) Mesh final
	{
		Vtxs vtxs;
		Inds inds;

		mat4 trns;
	};

	typedef std::vector<Mesh> Meshs;

	struct Thread final
	{
		Tasks on_init;
		Tasks on_exec;
		Tasks on_once;
		Tasks on_kill;

		bool KILL = false;
		std::thread thread;
	};
	//
};
