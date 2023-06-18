#pragma once

#include "Loom.h"
#include "Enums.h"

#include <glm/glm.hpp>
using namespace glm;

#include <mutex>
#include <thread>
#include <atomic>
#include <memory>
#include <vector>
#include <string>
#include <iostream>

namespace loom
{
	// Getting around thread_local bullshit
	struct ShaderManager;
	ShaderManager* GetSMgr();
	//


	// GameObjects
	template <typename T>
	struct GameObject
	{
	public:
		friend struct Engine;
		GameObject(bool self_add = true)
		{
			if (self_add)
			{
				std::scoped_lock<std::recursive_mutex> lock{mut};
				objects.push_back(static_cast<T*>(this));
			};
		};
		virtual ~GameObject()
		{
			std::scoped_lock<std::recursive_mutex> lock{mut};
			objects.erase(std::remove(objects.begin(), objects.end(), static_cast<T*>(this)), objects.end());
		};
		static void add(T* t)
		{
			std::scoped_lock<std::recursive_mutex> lock{mut};
			objects.push_back(t);
		};
		static void access(void(*f)(T&))
		{
			std::scoped_lock<std::recursive_mutex> lock{mut};
			for (auto& object : objects)
				f(*object);
		};
		static void clear()
		{
			std::scoped_lock<std::recursive_mutex> lock{mut};
			objects.clear();
		};

		operator T*() { return static_cast<T*>(this); };
		 
	protected:
		static inline std::recursive_mutex mut;
		static inline std::vector<T*> objects;
	};
	struct Loadable : public GameObject<Loadable>
	{
	protected:
		friend struct Engine;
		virtual void load()=0;
	};
	struct Renderable : public GameObject<Renderable>
	{
	protected:
		friend struct Engine;
		friend struct Camera;
		virtual void render()=0;
	};
	struct Updateable : public GameObject<Updateable>
	{
	protected:
		friend struct Engine;
		virtual void update()=0;
	};
	struct Unloadable : public GameObject<Unloadable>
	{
	protected:
		friend struct Engine;
		virtual void unload()=0;
	};


	// Shader Object
	struct Shader :
		virtual protected Loadable,
		virtual protected Unloadable
	{
		// TODO: Make custom class for this, move to new file
		// TODO: Push ShaderManager into this

		uint32_t id = 0;

		Shader(std::string files...)
			: files({ files })
		{ };

	private:
		void load();
		void unload() { };

		std::vector<std::string> files;
	};


	// Texture Object
	struct Texture :
		virtual protected Loadable,
		virtual protected Unloadable
	{
		// TODO: Make custom class for this, move to new file

		uint32_t id = 0;
		int32_t w = -1, h = -1;

		Texture(std::string file, uint32_t type);

	private:
		void load();
		void unload() { };

		std::function<void()> f;
		std::string file;
		uint32_t type = 0;
		int32_t nrChannels = -1;
	};
};
