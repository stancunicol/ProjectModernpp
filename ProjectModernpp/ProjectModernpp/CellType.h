#pragma once
#include <cstdint>
enum class CellType : uint8_t
{
	EMPTY,//0
	PLAYER,//1
	ENEMY,//2
	BREAKABLE_WALL,//3
	UNBREAKABLE_WALL,//4
	BASE,//5
	BULLET//6
};