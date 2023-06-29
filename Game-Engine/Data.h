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


namespace loom
{
	template <typename T> struct Manager;

	struct Updateable;
	struct Renderable;

	template<typename T>
		concept _render_test = requires
	{
		{ std::declval<T>().render() } -> std::same_as<void>;
	};
	
	template<typename T>
	concept _update_test = requires
	{
		{ std::declval<T>().update() } -> std::same_as<void>;
	};

	template<typename T, typename S>
	concept _container_test = requires
	{
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
			assert((!is_in_engine || !Engine::GetIsRunning(),
				"It's dangerous to add something to the engine before it's done constructing!"));
		};
		virtual ~GameObject()
		{
			assert((!is_in_engine || !Engine::GetIsRunning(),
				"It's dangerous to delete something before it's done being removed from the engine!"));
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
		template <typename T>
		friend struct ptr;
		
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


	// Works like a std::shared_ptr, guarantees memory safe lifetime management inside the engine
	// Automatically adds and removes itself based on lifetime
	template <typename T>
	struct ptr : std::shared_ptr<T>
	{
		ptr() : std::shared_ptr<T>(nullptr)
		{

		};
		ptr(T* t) : std::shared_ptr<T>(t)
		{

		};
		ptr(bool nothing, auto&&... args) : std::shared_ptr<T>(new T(args...))
		{
			if constexpr (std::is_base_of<GameObject<T>, T>::value)
				(*this)->GameObject<T>::AddToEngine();
			if constexpr (std::is_base_of<Updateable, T>::value)
				(*this)->GameObject<Updateable>::AddToEngine();
			if constexpr (std::is_base_of<Renderable, T>::value)
				(*this)->GameObject<Renderable>::AddToEngine();
		};
		virtual ~ptr()
		{
			if (std::shared_ptr<T>::use_count() == 1)
			{
				if constexpr (std::is_base_of<GameObject<T>, T>::value)
					(*this)->GameObject<T>::RmvFromEngine();
				if constexpr (std::is_base_of<Updateable, T>::value)
					(*this)->GameObject<Updateable>::RmvFromEngine();
				if constexpr (std::is_base_of<Renderable, T>::value)
					(*this)->GameObject<Renderable>::RmvFromEngine();
			};
		};
	};
};
