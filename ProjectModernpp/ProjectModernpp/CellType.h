#pragma once
#include <cstdint>
enum class CellType : uint8_t
{
	EMPTY = '.',
	BREAKABLE_WALL = '*',
	UNBREAKABLE_WALL = '#'
};