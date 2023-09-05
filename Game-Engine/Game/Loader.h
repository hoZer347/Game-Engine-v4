#pragma once

namespace loom
{
	typedef unsigned char uint8;

	struct Container;
	template <typename T> struct ptr;

	// Loads the map of the given ID, returns a ptr<Container> that maintains ownership of it
	template <uint8 ID>
	ptr<Container> LoadMap();

	// Loads the scene of the given ID, returns a ptr<Container> that maintains ownership of it
	template <uint8 ID>
	ptr<Container> LoadScene();
};
