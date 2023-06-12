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
		friend struct Loom;
		GameObject()
		{
			objects.push_back(static_cast<T*>(this));
		};
		virtual ~GameObject()
		{ };

		operator T*() { return static_cast<T*>(this); };

		static inline std::vector<T*> objects;
	};
	struct Loadable : public GameObject<Loadable>
	{
	protected:
		friend struct Loom;
		virtual void load()=0;
		static void load_all()
		{
			for (auto i = 0; i < objects.size(); i++)
				objects[i]->load();

			GameObject<Loadable>::objects.clear();
		};
	};
	struct Renderable : public GameObject<Renderable>
	{
	protected:
		friend struct Loom;
		friend struct Camera;
		virtual void render()=0;
		static void render_all()
		{
			for (auto i = 0; i < objects.size(); i++)
				objects[i]->render();
		};
	};
	struct Updateable : public GameObject<Updateable>
	{
	protected:
		friend struct Loom;
		virtual void update()=0;
		static void update_all()
		{
			for (auto i = 0; i < objects.size(); i++)
				objects[i]->update();
		};
	};
	struct Unloadable : public GameObject<Unloadable>
	{
	protected:
		friend struct Loom;
		virtual void unload()=0;
		static void unload_all()
		{
			for (auto i = 0; i < objects.size(); i++)
				objects[i]->unload();

			GameObject<Unloadable>::objects.clear();
		};
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
