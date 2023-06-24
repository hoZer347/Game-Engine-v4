#pragma once

#include "Engine.h"
#include "Utils.h"

#include <glm/glm.hpp>
using namespace glm;

#include <mutex>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <type_traits>


namespace loom
{
	template <typename T> struct Manager;

	struct Updateable;
	struct Renderable;

	template<typename T>
		concept _render_test = requires {
	{ std::declval<T>().render() } -> std::same_as<void>;
	};
	
	template<typename T>
	concept _update_test = requires {
		{ std::declval<T>().update() } -> std::same_as<void>;
	};

	template<typename T, typename S>
	concept _container_test = requires {
		{ std::declval<T>().begin() } -> std::same_as<S&>;
		{ std::declval<T>().end() } -> std::same_as<S&>;
	};

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
		virtual void update() = 0;
	};

	inline void Engine::Add()
	{ };
	template <typename ARG, typename... ARGS>
	inline void Engine::Add(ARG* arg, ARGS&&... args)
	{
		if constexpr (std::is_base_of<GameObject<ARG>, ARG>::value)
			arg->GameObject<ARG>::AddToEngine();
		if constexpr (std::is_base_of<Updateable, ARG>::value)
			arg->GameObject<Updateable>::AddToEngine();
		if constexpr (std::is_base_of<Renderable, ARG>::value)
			arg->GameObject<Renderable>::AddToEngine();

		Add(args...);
	};


	inline void Engine::Rmv()
	{ };
	template <typename ARG, typename... ARGS>
	inline void Engine::Rmv(ARG* arg, ARGS&&... args)
	{
		if constexpr (std::is_base_of<GameObject<ARG>, ARG>::value)
			arg->GameObject<ARG>::RmvFromEngine();
		if constexpr (std::is_base_of<Updateable, ARG>::value)
			arg->GameObject<Updateable>::RmvFromEngine();
		if constexpr (std::is_base_of<Renderable, ARG>::value)
			arg->GameObject<Renderable>::RmvFromEngine();

		Rmv(args...);
	};


	template<std::size_t n1, std::size_t ...args>
	struct mul
	{
		static constexpr std::size_t value = n1 * mul<args...>::value;
	};
	template<std::size_t n>
	struct mul<n>
	{
		static constexpr std::size_t value = n;
	};
	template<std::size_t n1, std::size_t ...args>
	struct add
	{
		static constexpr std::size_t value = n1 + mul<args...>::value;
	};
	template<std::size_t n>
	struct add<n>
	{
		static constexpr std::size_t value = n;
	};


	template<size_t N>
	struct Str {
		constexpr Str(const char(&str)[N]) {
			std::copy_n(str, N, value);
		}
		char value[N];
	};
	template<Str lit>
	void Print() {
		// The size of the string is available as a constant expression.
		constexpr auto size = sizeof(lit.value);

		// and so is the string's content.
		constexpr auto contents = lit.value;

		std::cout << "Size: " << size << ", Contents: " << contents << std::endl;
	};


	// Guarantees memory safe lifetime management
	// Works like a std::shared_ptr
	template <typename T>
	struct ptr : std::shared_ptr<T>
	{
		template <typename... ARGS>
		ptr(ARGS... args) : std::shared_ptr<T>(new T(args...))
		{
			Engine::Add(std::shared_ptr<T>::get());
		};
		virtual ~ptr()
		{
			Engine::Rmv(std::shared_ptr<T>::get());
		};
	};
};
