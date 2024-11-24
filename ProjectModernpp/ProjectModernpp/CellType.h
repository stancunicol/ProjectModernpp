#pragma once
#include <cstdint>
enum class __declspec(dllexport) CellType : uint8_t
{
	EMPTY,
	PLAYER,
	ENEMY,
	BREAKABLE_WALL,
	UNBREAKABLE_WALL,
	BASE,
	BULLET
};