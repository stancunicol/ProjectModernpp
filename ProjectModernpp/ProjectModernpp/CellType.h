#pragma once
#include <cstdint>
enum class CellType : uint8_t
{
	EMPTY = '.',//0
	PLAYER = 'P',//1
	ENEMY = 'E',//2
	BREAKABLE_WALL = '*',//3
	UNBREAKABLE_WALL = '#',//4
	BASE = 'B',//5
	BULLET = 'o'//6
};