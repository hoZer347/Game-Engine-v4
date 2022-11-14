#pragma once

#include <mutex>
#include <vector>
#include <memory>
#include <functional>

namespace eng
{
	
	// Creates and maintains a thread-safe data set in a vector
	template <typename T, size_t ID=0>
	struct Data : public std::shared_ptr<T>
	{
	public:
		
		// Creates new object and returns its pointer
		static std::shared_ptr<T> create()
		{
			Data<T, ID>::mut.lock();
			Data<T, ID>::vec.emplace_back(new T());
			std::shared_ptr<T> ret = std::shared_ptr<T>(Data<T, ID>::vec.back());
			Data<T, ID>::mut.unlock();

			return ret;
		};

		// Creates new object(s) and returns its index in the memory vector
		static size_t create(size_t amount)
		{
			Data<T, ID>::mut.lock();
			Data<T, ID>::vec.reserve(amount);

			size_t ret = Data<T, ID>::vec.size();

			for (; amount > 0; amount--)
				Data<T, ID>::vec.emplace_back(new T());

			Data<T, ID>::mut.unlock();

			return ret;
		};

		// Access the memory vector inside this context using a lambda
		// ProTip: sub out std::vector<std::shared_ptr<T>>& for "auto"
		static void access(std::function<void(std::vector<std::shared_ptr<T>>&)> f)
		{
			Data<T, ID>::mut.lock();
			f(Data<T, ID>::vec);
			Data<T, ID>::mut.unlock();
		};

		// Gets the amount of objects in this data source
		inline static size_t size()
		{
			size_t ret=0;
			Data<T, ID>::mut.lock();
			for (auto i : Data<T, ID>::vec)
				if (i)
					ret++;
			Data<T, ID>::mut.unlock();

			return ret;
		};

	protected:
		Data() { };

		static inline std::mutex						mut;
		static inline std::vector<std::shared_ptr<T>>	vec;
	};

	template <typename T, size_t ID=0>
	struct Buffer
	{
		// Creates an amount of objects (default 1)
		static void create(size_t amount=1)
		{
			mut.lock();
			muteable.reserve(amount);
			immuteable.reserve(amount);

			for (; amount > 0; amount--)
			{
				muteable.emplace_back(new T());
				immuteable.emplace_back(new T());
			};
			mut.unlock();
		};

		// Gives access the the immuteable vector
		static const std::vector<std::shared_ptr<T>>& view_vec()
		{
			mut.lock();
			const std::vector<std::shared_ptr<T>>& ret = immuteable;
			mut.unlock();

			return ret;
		};

		// Executes a function over the muteable vector
		static void access(std::function<void(std::vector<std::shared_ptr<T>>&)> f)
		{
			mut.lock();
			f(muteable);
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
		static inline std::mutex mut;
		static inline std::vector<std::shared_ptr<T>> muteable, immuteable;
	};
};
