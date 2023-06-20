#pragma once

#include "Engine.h"

#include <glm/glm.hpp>
using namespace glm;

#include <mutex>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <type_traits>

template<typename T>
concept has_render_member_func = requires {
	{ std::declval<T>().render() } -> std::same_as<void>;
};

template<typename T>
concept has_update_member_func = requires {
	{ std::declval<T>().update() } -> std::same_as<void>;
};

namespace loom
{
	template <typename T> struct Manager;

	struct Updateable;
	struct Renderable;

	
	// GameObjects
	template <typename T>
	struct GameObject
	{
	public:
		GameObject()
		{
			int i = 0;
			assert(!is_in_engine || !Engine::GetIsRunning());
		};
		virtual ~GameObject()
		{
			assert(!is_in_engine || !Engine::GetIsRunning());
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
		template <typename Base>
		friend struct Manager;
		friend struct Engine;
		void AddToEngine()
		{
			std::scoped_lock<std::recursive_mutex> lock{mut};
			is_in_engine = true;
			objects.push_back(static_cast<T*>(this));
		};
		void RmvFromEngine()
		{
			std::scoped_lock<std::recursive_mutex> lock{mut};
			is_in_engine = false;
			std::erase_if(objects, [this](T* t) { return t == static_cast<T*>(this); });
		};

		bool is_in_engine = false;

		static inline std::recursive_mutex mut;
		static inline std::vector<T*> objects;
	};
	struct Renderable :
		public GameObject<Renderable>
	{
	protected:
		friend struct Engine;
		virtual void render()=0;
	};
	struct Updateable :
		public GameObject<Updateable>
	{
	protected:
		friend struct Engine;
		virtual void update()=0;
	};


	inline void Engine::Add()
	{ };
	template <typename T, typename... ARGS>
	inline void Engine::Add(T* t, ARGS... to_add)
	{
		if constexpr (std::is_base_of<GameObject<T>, T>::value)
			t->GameObject<T>::AddToEngine();
		if constexpr (std::is_base_of<Updateable, T>::value)
			t->GameObject<Updateable>::AddToEngine();
		if constexpr (std::is_base_of<Renderable, T>::value)
			t->GameObject<Renderable>::AddToEngine();

		Add(to_add...);
	};


	inline void Engine::Rmv()
	{ };
	template <typename T, typename... ARGS>
	inline void Engine::Rmv(T* t, ARGS... to_add)
	{
		if constexpr (std::is_base_of<GameObject<T>, T>::value)
			t->GameObject<T>::RmvFromEngine();
		if constexpr (std::is_base_of<Updateable, T>::value)
			t->GameObject<Updateable>::RmvFromEngine();
		if constexpr (std::is_base_of<Renderable, T>::value)
			t->GameObject<Renderable>::RmvFromEngine();

		Rmv(to_add...);
	};
};
