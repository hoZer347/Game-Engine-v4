#pragma once

#include <mutex>
#include <vector>
#include <memory>
#include <functional>

#include <iostream>

#ifdef LOG_DATA_CREATE
#define LOG_CREATE(x) std::cout << "Created " << typeid(T).name() << ": " << x << std::endl
#define LOG_CREATES(x) std::cout << "Created " << x << " of " << typeid(T).name() << std::endl
#else
#define LOG_CREATE(x)
#define LOG_CREATES(x)
#endif

namespace eng
{
	// Creates and maintains a thread-safe data set in a vector
	template <typename T, size_t ID=0>
	struct Data
	{
		// Creates new object and returns its pointer
		static std::shared_ptr<T> create()
		{
			Data<T, ID>::mut.lock();	
			Data<T, ID>::vec.emplace_back(new T());
			std::shared_ptr<T> ret = std::shared_ptr<T>(Data<T, ID>::vec.back());
			
			LOG_CREATE(ret);
			Data<T, ID>::mut.unlock();

			return ret;
		};

		// Creates new object(s) and returns its index in the memory vector
		static size_t create(size_t amount)
		{
			Data<T, ID>::mut.lock();

			Data<T, ID>::vec.reserve(amount);
			LOG_CREATES(amount);

			size_t ret = Data<T, ID>::vec.size();

			for (; amount > 0; amount--)
				Data<T, ID>::vec.emplace_back(new T());

			Data<T, ID>::mut.unlock();

			return ret;
		};

		// Access the memory vector inside this context using a lambda
		static void modify(std::function<void(std::vector<std::shared_ptr<T>>&)> f)
		{
			Data<T, ID>::mut.lock();
			f(Data<T, ID>::vec);
			Data<T, ID>::mut.unlock();
		};

		// Iterates over memory vector, executes function over every element
		static void access(std::function<void(std::shared_ptr<T>&)> f)
		{
			Data<T, ID>::mut.lock();
			for (auto& i : vec)
				f(i);
			Data<T, ID>::mut.unlock();
		};

		// Gets the amount of objects in this data source
		inline static size_t size()
		{
			size_t ret=0;
			Data<T, ID>::mut.lock();
			ret = vec.size();
			Data<T, ID>::mut.unlock();

			return ret;
		};

	private:
		Data() { };

		static inline std::mutex						mut;
		static inline std::vector<std::shared_ptr<T>>	vec;
	};

	// Duplicates memory for safe parallel rendering and updating
	template <typename T, size_t ID=0>
	struct Buffer
	{
		// Creates an amount of objects (default 1)
		static void create(size_t amount=1)
		{
			mut.lock();
			muteable.reserve(amount);
			LOG_CREATES(amount);
			immuteable.reserve(amount);
			LOG_CREATES(amount);

			for (; amount > 0; amount--)
			{
				muteable.emplace_back(new T());
				immuteable.emplace_back(new T());
			};
			mut.unlock();
		};

		// Executes a function over the muteable vector
		static void modify(std::function<void(std::vector<std::shared_ptr<T>>&)> f)
		{
			mut.lock();
			f(muteable);
			mut.unlock();
		};

		// Executes a function over every element in the muteable vector
		static void access(std::function<void(std::shared_ptr<T>&)> f)
		{
			mut.lock();
			for (auto& i : muteable)
				f(i);
			mut.unlock();
		};

		// Executes a function over every element in the immuteable vector
		static void view(std::function<void(const std::shared_ptr<T>&)> f)
		{
			mut.lock();
			for (auto& i : immuteable)
				f(i);
			mut.unlock();
		};

		// Swaps the muteable and immutable vectors
		static void swap()
		{
			mut.lock();
			muteable.swap(immuteable);
			mut.unlock();
		};

	private:
		Buffer() { };

		static inline std::mutex						mut;
		static inline std::vector<std::shared_ptr<T>>	muteable, immuteable;
	};
};
