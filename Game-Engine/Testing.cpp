#include "Testing.h"

#include <iostream>

#include "Sprite.h"
#include "Texture.h"

namespace loom
{
	void InheritanceOrderTest()
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



};
