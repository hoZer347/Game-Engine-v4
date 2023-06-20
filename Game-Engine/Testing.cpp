#include "Testing.h"

#include <iostream>

#include "Sprite.h"
#include "Texture.h"

namespace loom
{
	inline void InheritanceOrderTest()
	{
		struct Base
		{
			Base() { std::cout << "Constructing: " << typeid(*this).name() << std::endl; };
			~Base() { std::cout << "Deleting: " << typeid(*this).name() << std::endl; };
		};

		struct Derived0 : Base
		{
			Derived0() { std::cout << "Constructing: " << typeid(*this).name() << std::endl; };
			~Derived0() { std::cout << "Deleting: " << typeid(*this).name() << std::endl; };
		};

		struct Derived1 : Derived0
		{
			Derived1() { std::cout << "Constructing: " << typeid(*this).name() << std::endl; };
			~Derived1() { std::cout << "Deleting: " << typeid(*this).name() << std::endl; };
		};

		{
			Derived1 derived;
		};
	};


	void SpriteTest()
	{
		Engine::Init();

		Camera camera;
		Control control;
		Camera::InitiateFreeCam(control);

		Texture texture{ "Resources/Anna.png", GL_RGBA };
		Sprite sprite{ texture, vec2{ 5 * 32, 32 * 11 }, vec2{ 32, 32 }, vec2{ 0, 0 }, 0 };

		Engine::Add(&texture, &sprite, &camera, &control);

		Engine::Run();
	};
};
