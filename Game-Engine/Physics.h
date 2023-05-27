#pragma once

#include "Data.h"

namespace loom
{
	template <typename S, typename T, uint64_t ID=0>
	struct OnCollisionFunction final
	{
		OnCollisionFunction(void(*f)(S&, T&))
		{ this->f = f; };

		static inline void(*f)(S&, T&) = {};

	private:
		OnCollisionFunction() { };
	};


	template <typename S, typename T, uint64_t ID=0>
	struct CollisionCompareFunction final
	{
		CollisionCompareFunction(bool(*f)(S&, T&))
		{ this->f = f; };

		static inline bool(*f)(S&, T&) = nullptr;

	private:
		CollisionCompareFunction() { };
	};
	
	
	template <typename T, uint64_t ID=0>
	struct Collision final
	{
		template <typename... ARGS>
		Collision(ARGS... args) : obj(args...)
		{ };

		operator T& () { return obj; };



	private:
		T obj;
		mat4& trns = obj.trns;

		struct Manager final :
			virtual protected Physicsable,
			virtual protected Updateable,
			virtual protected Renderable
		{
		private:
			void update() override
			{ };
			void physics() override
			{ };
			void render() override
			{ };
		};

		static inline Manager manager;
	};



	template <typename T>
	struct Physics final :
		virtual protected Physicsable,
		virtual protected Updateable
	{
		template <typename... ARGS>
		Physics(ARGS... args) : obj(args...)
		{ };

		void physics() override
		{ };
		void update()
		{ };

		T obj;
	};
};
