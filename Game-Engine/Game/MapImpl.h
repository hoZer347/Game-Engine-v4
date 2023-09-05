#pragma once

#include "../Data.h"

#include <unordered_set>

namespace loom
{
	struct Unit;
	struct Cell;

	// Condensed version of what Map uses
	struct MapImpl
	{
		// Determines the heights of each cell in terms of z = height_func(x, y)
		float(*height_func)(float x, float y, const uint64& time_elapsed) = [](float, float, const uint64&) { return 1.f; };

		// Cell that is currently being hovered by the mouse
		Cell* hovered = nullptr;

		// Resets all highlights on the map
		virtual void clear_highlights() = 0;

		// Sets highlights (x0, y0) -> (x1, y1) to the given value
		virtual void fill_highlights(uint16 value, uint16 x0, uint16 y0, uint16 x1, uint16 y1) = 0;

		// Floodfills from (x, y) using cell connections (not adjacency), cells in provided set don't count
		virtual void floodfill_hightlights(const uint8& value, float amount, std::unordered_set<Cell*>& cells) = 0;

		// Connects adjacent cells (x0, y0) -> (x1, y1) to adjecent cells
		virtual void connect_adjacent(uint16 x0, uint16 y0, uint16 x1, uint16 y1) = 0;

		// Adds a given unit to the map at (x, y)
		virtual void set_unit(ptr<Unit> unit, uint16 x, uint16 y) = 0;

		// Generates a mesh based on the given changes
		virtual void generate() = 0;

		// Detects highlights on the map cells and generates an index list based on it
		virtual void refresh_highlights() = 0;

		// Sets heights at (x0, y0) -> (x1, y1) to the given value
		virtual void fill_heights(uint16 value, uint16 x0, uint16 y0, uint16 x1, uint16 y1) = 0;

		// Iterates through cells to find the one the player is hovering with their mouse (output is in this->hovered)
		virtual void find_hovered() = 0;
	};
};
