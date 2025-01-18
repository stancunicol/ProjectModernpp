#include "RandomMove.h"
#include <iostream>

RandomMove::RandomMove()
{
    ShuffleDirections();
}

void RandomMove::ShuffleDirections()
{
    if (directions.empty()) { std::cerr << "Error: Directions vector is empty before shuffle!\n"; return; }

    std::shuffle(directions.begin(), directions.end(), std::mt19937(std::random_device{}()));
    currentIndex = 0;
}

std::pair<int8_t, int8_t> RandomMove::GenerateNextDirection()
{
    if (directions.empty()) { 
        std::cerr << "Error: Directions vector is empty!\n"; 
        return {0, 0}; 
    }
    if (currentIndex >= directions.size()) {
        ShuffleDirections();
    }

    auto direction = directions[currentIndex];
    currentIndex++;

    return direction;
}
