#pragma once

#include <vector>
#include <iostream>

namespace loom
{
	struct Mesh final
	{
		void** data = nullptr;
	};

	template <typename S, typename T> struct Att;
	template <typename S> struct _Att;

	template <typename S>
	struct _Att final
	{
		static inline void Load(Mesh& mesh)
		{
			mesh.data = (void**)malloc(count * sizeof(void*));

			for (auto i = 0; i < count; i++)
				mesh.data[i] = malloc(sizes[i]);
		};

		static inline void Free(Mesh& mesh)
		{
			for (auto i = 0; i < count; i++)
				free(mesh.data[i]);

			free(mesh.data);
		};

	protected:
		template <typename S, typename T>
		friend struct Att;

		static inline size_t count = 0;
		static inline std::vector<size_t> sizes;
	};

	template <typename S, typename T>
	struct Att final
	{
		static T& Get(Mesh& mesh)
		{ return *(T*)mesh.data[index]; };

	protected:
		template <typename S>
		friend struct _Att;

	private:
		Att() { };
		
		static inline size_t initializer()
		{
			if (claimed)
			{
				std::cerr << "This type has already been claimed in this context" << std::endl;
				exit(0);
			};
			_Att<S>::sizes.push_back(sizeof(T));
			claimed = true;
			return _Att<S>::count++;
		};

		static inline bool claimed = false;
		static inline const size_t& index = initializer();
	};
};
