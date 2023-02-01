#pragma once

#include <chrono>
#include <atomic>
#include <functional>
#include <type_traits>

#define TESTFOR(NAME)\
	template<typename T, typename = void>\
	struct has##NAME : std::false_type\
	{};\
	template<typename T>\
	struct has##NAME<T, decltype(std::declval<T>().##NAME(), void())> : std::true_type\
	{};\
	static inline const bool Has##NAME = has##NAME<T>::value;



namespace loom
{
	typedef void(*Task)();

	struct Utils
	{
	private:
		// Test whether or not a class has certain functions
		template <typename T>
		struct HasFuncTest
		{
			TESTFOR(load);
			TESTFOR(sync);
			TESTFOR(update);
			TESTFOR(render);
			TESTFOR(unload);
		};
		//

		typedef std::chrono::high_resolution_clock Clock;
		typedef std::chrono::steady_clock::time_point Time;	
		
		template <typename T>
		struct Data final
		{
		protected:
			friend struct Utils;
			static inline std::vector<T*> data;
		};



	protected:
		friend struct Loom;
		friend struct Camera;
		friend struct Perspective;
		static inline std::vector<void(*)()> loads;
		static inline std::vector<void(*)()> syncs;
		static inline std::vector<void(*)()> updates;
		static inline std::vector<void(*)()> renders;
		static inline std::vector<void(*)()> unloads;
		static inline std::vector<std::function<void()>> invks;



	public:
		// High Resolution Timer so you don't have to interact with std::chrono lmao
		struct Timer
		{
			[[nodiscard]] const double GetDiff_s()   const { return diff / 1000000000; };
			[[nodiscard]] const double GetDiff_mcs() const { return diff / 1000000; };
			[[nodiscard]] const double GetDiff_mls() const { return diff / 1000; };
			[[nodiscard]] const double GetDiff_ns()  const { return diff; };

			void update()
			{
				diff = (double)(Clock::now() - _clock).count();
				_clock = Time(Clock::now());
			};

		private:
			Time _clock = Clock::now();
			double diff = 0;
		};
		//
		


		// TODO: Get Construct / Deconstruct to run on a unique Helper thread (i.e. the loader thread)

		// Construction of object T
		template <typename T, typename... ARGS>
		static std::unique_ptr<T> Construct(ARGS&&... args)
		{
			T* _t = new T(args...);

			invks.emplace_back([=]
			{
				static auto b = []()
				{
					if constexpr (HasFuncTest<T>::Hasload)
						loads.emplace_back([]()
						{
							for (auto& t : Data<T>::data)
								if (t)
									t->load();
						});
					
					if constexpr (HasFuncTest<T>::Hassync)
						syncs.emplace_back([]()
						{
							for (auto& t : Data<T>::data)
								if (t)
									t->sync();
						});
					
					if constexpr (HasFuncTest<T>::Hasupdate)
						updates.emplace_back([]()
						{
							for (auto& t : Data<T>::data)
								if (t)
									t->update();
						});
					
					if constexpr (HasFuncTest<T>::Hasrender)
						renders.emplace_back([]()
						{
							for (auto& t : Data<T>::data)
								if (t)
									t->render();
						});

					if constexpr (HasFuncTest<T>::Hasunload)
						unloads.emplace_back([]()
						{
							for (auto& t : Data<T>::data)
								if (t)
									t->unload();
						});

					return nullptr;
				};
				static void* _b = b();
				Data<T>::data.emplace_back(_t);
			});

			return std::unique_ptr<T>(_t);
		};
		//



		// Deconstruction of object T
		template <typename T>
		static void Deconstruct(std::unique_ptr<T>& t)
		{
			T* _t = t.get();

			invks.emplace_back([=]()
			{
				Data<T>::data.erase(
					std::remove(
						Data<T>::data.begin(),
						Data<T>::data.end(),
						_t));
			});

			t = nullptr;
		};
		//
	};
};
