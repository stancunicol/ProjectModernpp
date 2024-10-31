#pragma once
#include "CellType.h"
#include <cstdint>
#include <vector>

class GameMap
{
private:
	uint32_t width, height;
	std::vector<std::vector<CellType>> grid;

public:
	GameMap(int w, int h) : width(w), height(h) {
		grid.resize(height, std::vector<CellType>(width, CellType::EMPTY));
	}

};