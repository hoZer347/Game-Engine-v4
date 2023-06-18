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
#include <type_traits>

namespace loom
{
	// Getting around thread_local bullshit
	struct ShaderManager;
	ShaderManager* GetSMgr();
	//


	// Works like an std::unique_ptr, but guarantees certain memory safties within the engine
	// Modifying certain data during initialization is safe
	// Create and Deletion are both safe
	template <typename T>
	struct ptr : std::unique_ptr<T>
	{
		template <typename... ARGS>
		ptr(ARGS... args) : std::unique_ptr<T>(new T(args...))
		{
			Engine::Add(std::unique_ptr<T>::get());
		};
		~ptr()
		{
			Engine::Rmv(std::unique_ptr<T>::get());
		};
	};


	// GameObjects
	template <typename T>
	struct GameObject
	{
	public:
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
		friend struct Engine;
		friend struct ptr<T>;
		void AddToEngine()
		{
			std::scoped_lock<std::recursive_mutex> lock{mut};
			objects.push_back(static_cast<T*>(this));
		};
		void RmvFromEngine()
		{
			std::scoped_lock<std::recursive_mutex> lock{mut};
			std::erase_if(objects, [this](T* t) { return t == static_cast<T*>(this); });
		};

		static inline std::recursive_mutex mut;
		static inline std::vector<T*> objects;
	};
	struct Loadable :
		public GameObject<Loadable>
	{
	protected:
		friend struct Engine;
		virtual void load()=0;
	};
	struct Renderable :
		public GameObject<Renderable>
	{
	protected:
		friend struct Engine;
		friend struct Camera;
		virtual void render()=0;
	};
	struct Updateable :
		public GameObject<Updateable>
	{
	protected:
		friend struct Engine;
		virtual void update()=0;
	};
	struct Unloadable :
		public GameObject<Unloadable>
	{
	protected:
		friend struct Engine;
		virtual void unload()=0;
	};


	inline void Engine::Add()
	{ };
	template <typename T, typename... ARGS>
	inline void Engine::Add(T* t, ARGS... to_add)
	{
		if constexpr (std::is_base_of<Loadable, T>::value)
			t->GameObject<Loadable>::AddToEngine();
		if constexpr (std::is_base_of<Updateable, T>::value)
			t->GameObject<Updateable>::AddToEngine();
		if constexpr (std::is_base_of<Renderable, T>::value)
			t->GameObject<Renderable>::AddToEngine();
		if constexpr (std::is_base_of<Unloadable, T>::value)
			t->GameObject<Unloadable>::AddToEngine();

		Add(to_add...);
	};


	inline void Engine::Rmv()
	{ };
	template <typename T, typename... ARGS>
	inline void Engine::Rmv(T* t, ARGS... to_add)
	{
		if constexpr (std::is_base_of<Loadable, T>::value)
			t->GameObject<Loadable>::RmvFromEngine();
		if constexpr (std::is_base_of<Updateable, T>::value)
			t->GameObject<Updateable>::RmvFromEngine();
		if constexpr (std::is_base_of<Renderable, T>::value)
			t->GameObject<Renderable>::RmvFromEngine();
		if constexpr (std::is_base_of<Unloadable, T>::value)
			t->GameObject<Unloadable>::RmvFromEngine();

		Rmv(to_add...);
	};


	// Shader Object
	struct Shader final :
		virtual public Loadable,
		virtual public Unloadable
	{
		// TODO: Make custom class for this, move to new file
		// TODO: Push ShaderManager into this

		uint32_t id = 0;

		Shader(std::string files...)
			: files({ files })
		{
			Engine::Add(this);
		};

	private:
		void load();
		void unload() { };

		std::vector<std::string> files;
	};


	// Texture Object
	struct Texture final :
		virtual public Loadable,
		virtual public Unloadable
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
