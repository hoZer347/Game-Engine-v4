#pragma once

#include "Task.h"

#include <mutex>
#include <vector>
#include <functional>

namespace loom
{
	template <typename T, size_t ID>
	struct Buffer
	{
		typedef std::function<void(T&)>			Edit;
		typedef std::function<void(const T&)>	View;

		_NODISCARD static inline Task _allocate(size_t amount, auto... args)
		{
			return [amount, args...]()
			{
				_toAdd.reserve(amount);
				for (auto i = 0; i < amount; i++)
					_toAdd.emplace_back(args...);
			};
		};

		_NODISCARD static inline Task _edit(Edit edit)
		{
			return [edit]() noexcept
			{
				for (auto& t : _mutable)
					edit(t);
			};
		};

		_NODISCARD static inline Task _view(View view)
		{
			return [view]() noexcept
			{
				for (auto& t : _mutable)
					view(t);
			};
		};

		_NODISCARD static inline Task _swap()
		{
			return []() noexcept
			{
				_mutable.swap(_immutable);

				if (!_toAdd.empty())
				{
					_mutable.reserve(_toAdd.size());
					_immutable.reserve(_toAdd.size());
					_mutable.insert(_mutable.end(), _toAdd.begin(), _toAdd.end());
					_immutable.insert(_immutable.end(), _toAdd.begin(), _toAdd.end());
					_toAdd.clear();
				};
			};
		};

	private:
		static inline std::vector<T>	_toAdd;
		static inline std::vector<T>	_mutable;
		static inline std::vector<T>	_immutable;
	};
};
