#pragma once

#include <mutex>
#include <vector>
#include <memory>
#include <functional>

#include <iostream>

#ifdef LOG_DATA_CREATES
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

		// Creates new object(s) based on given args and returns its index in the memory vector
		static size_t create(size_t amount, auto... args)
		{
			Data<T, ID>::mut.lock();

			Data<T, ID>::vec.reserve(amount);
			LOG_CREATES(amount);

			size_t ret = Data<T, ID>::vec.size();

			for (; amount > 0; amount--)
				Data<T, ID>::vec.emplace_back(new T(args...));

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
			ret = Data<T, ID>::vec.size();
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
		// Creates an amount of objects, with given args
		static void create(size_t amount, auto... args)
		{
			Buffer<T, ID>::mut1.lock();
			Buffer<T, ID>::muteable.reserve(amount);
			LOG_CREATES(amount);
			Buffer<T, ID>::immuteable.reserve(amount);
			LOG_CREATES(amount);

			for (; amount > 0; amount--)
			{
				Buffer<T, ID>::muteable.emplace_back(new T(args...));
				Buffer<T, ID>::immuteable.emplace_back(new T(args...));
			};
			Buffer<T, ID>::mut1.unlock();
		};

		// Executes a function over the muteable vector
		static void modify(std::function<void(std::vector<std::shared_ptr<T>>&)> f)
		{
			Buffer<T, ID>::mut1.lock();
			f(Buffer<T, ID>::muteable);
			Buffer<T, ID>::mut1.unlock();
		};

		// Executes a function over every element in the muteable vector
		static void access(std::function<void(std::shared_ptr<T>&)> f)
		{
			Buffer<T, ID>::mut1.lock();
			for (auto& i : Buffer<T, ID>::muteable)
				f(i);
			Buffer<T, ID>::mut1.unlock();
		};

		// Executes a function over every element in the immuteable vector
		static void view(std::function<void(const std::shared_ptr<T>&)> f)
		{
			Buffer<T, ID>::mut2.lock();
			for (auto& i : Buffer<T, ID>::immuteable)
				f(i);
			Buffer<T, ID>::mut2.unlock();
		};

		// Swaps the muteable and immutable vectors
		static void swap()
		{
			Buffer<T, ID>::mut1.lock();
			Buffer<T, ID>::mut2.lock();
			Buffer<T, ID>::muteable.swap(Buffer<T, ID>::immuteable);
			Buffer<T, ID>::mut2.unlock();
			Buffer<T, ID>::mut1.unlock();
		};

	private:
		Buffer() { };

		static inline std::mutex						mut1, mut2;
		static inline std::vector<std::shared_ptr<T>>	muteable, immuteable;
	};
};
