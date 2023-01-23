#pragma once

#include <mutex>
#include <vector>
#include <functional>

namespace loom
{
	// Manage
	template <typename T>
	struct Manage
	{
	protected:
		friend struct Loom;
		friend struct Camera;
		Manage()
		{
			mut.lock();
			contents.push_back((T*)this);
			mut.unlock();
		};
		virtual ~Manage()
		{
			mut.lock();
			auto it = std::remove(contents.begin(), contents.end(), (T*)this);
			mut.unlock();
		};
		static void access(std::function<void(T*)> f)
		{
			mut.lock();
			for (auto& i : contents)
				f(i);
			mut.unlock();
		};
		static void clear()
		{
			mut.lock();
			contents.clear();
			mut.unlock();
		};

		static const size_t size() { return contents.size(); }

	private:
		static inline std::vector<T*> contents;
		static inline std::mutex mut;
	};
	//
};
