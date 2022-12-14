#pragma once

#include "Task.h"

#include <mutex>
#include <vector>
#include <functional>

namespace loom
{
	template <typename T>
	struct Buffer final
	{
	private:
		typedef std::function<void(T&)>			Edit;
		typedef std::function<void(const T&)>	View;

		std::vector<T>	_toAdd;
		std::vector<T>	_mutable;
		std::vector<T>	_immutable;

	public:
		Buffer()
		{ };

		_NODISCARD T& operator[](size_t i) { return _mutable[i]; }

		_NODISCARD Task _allocate(size_t amount, auto... args) noexcept
		{
			return [this, amount, args...]()
			{
				_toAdd.reserve(amount);
				for (auto i = 0; i < amount; i++)
					_toAdd.emplace_back(args...);
			};
		};

		_NODISCARD Task _edit(Edit edit) const noexcept
		{
			return [this, edit]()
			{
				for (auto& t : _mutable)
					edit(t);
			};
		};

		_NODISCARD Task _view(View view) const noexcept
		{
			return [this, view]()
			{
				for (auto& t : _mutable)
					view(t);
			};
		};

		_NODISCARD Task _update() noexcept
		{
			return [this]()
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
