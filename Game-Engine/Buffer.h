#pragma once

#include "Data.h"
#include <shared_mutex>

namespace loom
{
	struct __DoubleBuffer
	{
	protected:
		friend struct _DoubleBuffer;
		__DoubleBuffer() { };
		virtual void swap()=0;
		std::shared_mutex mut;
	};
	struct _DoubleBuffer final
	{
	protected:
		template <typename T>
		friend struct DoubleBuffer;
		static inline std::mutex _mut;
		static inline std::vector<__DoubleBuffer*> buffers;

	private:
		_DoubleBuffer() { };
		static inline std::thread thread = std::thread([]()
		{
			while (true)
			{
				_mut.lock();
				for (auto& buffer : buffers)
				{
					buffer->mut.lock();
					buffer->swap();
					buffer->mut.unlock();
				};
				_mut.unlock();
			};
		});
	};
	template <typename T = float>
	struct DoubleBuffer final : public __DoubleBuffer
	{
		DoubleBuffer()
		{
			_DoubleBuffer::_mut.lock();
			_DoubleBuffer::buffers.emplace_back(this);
			_DoubleBuffer::_mut.unlock();
		};
		virtual ~DoubleBuffer()
		{
			_DoubleBuffer::_mut.lock(); // Improve?
			std::remove_if(_DoubleBuffer::buffers.begin(), _DoubleBuffer::buffers.end(), [](DoubleBuffer* d) { return this == d });
			_DoubleBuffer::_mut.unlock();
		};

		void access1(void(*f)(std::vector<T>&))
		{
			mut.lock_shared();
			f(data1);
			mut.unlock_shared();
		};
		void access2(void(*f)(std::vector<T>&))
		{
			mut.lock_shared();
			f(data2);
			mut.unlock_shared();
		};

		// Add more functions for ease of use

	private:
		void swap()
		{
			if (long long diff = (long long)data1.size() - (long long)data2.size())
			{
				std::cout << diff << std::endl;

				if (diff > 0)
				{
					data2.reserve(diff);
					data2.insert(data2.end(), data1.end() - diff, data1.end());
				}
				else
				{
					data1.reserve(-diff);
					data1.insert(data1.end(), data2.end() + diff, data2.end());
				};
			};
			data1.swap(data2);
		};
		
		std::vector<T> data1;
		std::vector<T> data2;
	};
};
