#pragma once

#include <glm/glm.hpp>
using namespace glm;

#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <string>
#include <iostream>
#include <functional>

namespace loom
{
	// Generic Task Object
	// Basically works as an std::function, might make this a typedef
	struct Task
	{
		Task(auto task)
		: task(task)
		{ };

		Task(const Task& task)
		: task(task.task)
		{ };

		void operator()() { return task(); };
		std::function<void()> task;
	};
	//

	
	
	// Enumeration types
	typedef uint32_t ID;
	typedef uint32_t TYPE;
	//



	// Getting around thread_local bullshit
	struct ShaderManager;
	struct TextureManager;
	ShaderManager* GetSMgr();
	TextureManager* GetTMgr();
	//



	// Anything that should be loaded on run, and / or unloaded after, is a Object
	struct Object
	{
	protected: // TODO: Add Identification System
		friend struct Loom;
		Object();
		virtual ~Object();
		virtual void load()=0;
		virtual void unload()=0;
		static inline std::mutex mut;
		static inline std::vector<Object*> objects;
	};
	//



	// Anything that gets rendered is a Renderable
	struct Renderable
	{
	protected:
		friend struct Camera;
		Renderable();
		virtual ~Renderable();
		virtual void render()=0;
		static inline std::mutex mut;
		static inline std::vector<Renderable*> renderables;
	};
	//



	// Anything that is updated is an updatable
	struct Updatable
	{
	protected:
		friend struct Loom;
		Updatable();
		virtual ~Updatable();
		virtual void update()=0;
		static inline std::mutex mut;
		static inline std::vector<Updatable*> updatables;
	};
	//



	// Shader Object
	struct Shader final : public Object
	{
		Shader(std::string files...)
			: files({ files })
		{ };

		ID id = 0;

	private:
		void load() override;
		void unload() override;

		std::vector<std::string> files;
	};
	//



	// Texture Object
	struct Texture final : public Object
	{
		Texture(std::string file, TYPE type)
		: file(file), type(type), Object()
		{ };

		ID id = 0;

	private:
		void load() override;
		void unload() override;
		
		std::string file;
		TYPE type;
	};
	//
};
