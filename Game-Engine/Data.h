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
	struct Task
	{
		Task(auto task)
		: task(task)
		{ };

		void operator()() { return task(); };
		std::function<void()> task;
	};

	typedef uint32_t ID;
	typedef uint32_t TYPE;

	struct ShaderManager;
	struct TextureManager;

	ShaderManager* GetSMgr();
	TextureManager* GetTMgr();

	struct Object
	{
	protected: // TODO: Add Identification System
		friend struct Loom;
		Object();
		virtual ~Object();
		virtual void load()=0;
		virtual void unload()=0;
		static inline std::vector<Object*> objects;
	};



	struct Renderable : public Object
	{
	protected:
		friend struct Camera;
		Renderable();
		virtual ~Renderable();
		virtual void render()=0;
		static inline std::vector<Renderable*> renderables;
	};



	struct Updatable : public Object
	{
	protected:
		friend struct Loom;
		Updatable();
		virtual ~Updatable();
		virtual void update()=0;
		static inline std::vector<Updatable*> updatables;
	};



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

		void play() { STOP = false; };
		void stop() { STOP = true;  };
		void kill() { KILL = true;  };

	private:
		void load() override;
		void unload() override;

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



	struct Helper final
	{
		Helper();
		virtual ~Helper();
		static void assign(Task task);

	private:
		std::thread thread;
		std::atomic<bool> KILL;
		static inline std::mutex mut;
		static inline std::queue<Task> in;
		static inline std::atomic<int> PASS = 0;
	};
};
