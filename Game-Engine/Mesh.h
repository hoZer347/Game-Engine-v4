#pragma once

#include "Data.h"

#include <mutex>
#include <atomic>
#include <vector>
#include <functional>

namespace loom
{
	typedef unsigned int uint32;
	typedef std::vector<float>	Vtxs;
	typedef std::vector<uint32>	Inds;

	struct Texture;
	struct Shader;

	struct Mesh :
		public GameObject<Mesh>
	{
		Mesh(Vtxs&& _vtxs, Inds&& _inds);
		virtual ~Mesh();

		const uint32 min=0, max=0;

	private:
		template <typename T>
		struct Safe
		{
			void access(void(*f)(T&))
			{
				std::shared_ptr<T> _t;
				while (!(_t = data.exchange(nullptr)));

				f(*_t);

				data = _t;
			};

			std::shared_ptr<T> acquire()
			{
				std::shared_ptr<T> _t;
				while (!(_t = data.exchange(nullptr)));

				return _t;
			};

			void release(std::shared_ptr<T> _t)
			{
				data = _t;
			};

		private:
			std::atomic<std::shared_ptr<T>> data;
		};

		Safe<Inds> inds;
		static inline Safe<Vtxs> vtxs;
		static inline Safe<Inds> unused_inds;
	};
};
