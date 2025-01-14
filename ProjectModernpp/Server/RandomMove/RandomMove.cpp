#include "RandomMove.h"

RandomMove::RandomMove()
{
    ShuffleDirections();
}

void RandomMove::ShuffleDirections()
{
    std::shuffle(directions.begin(), directions.end(), std::mt19937(std::random_device{}()));
    currentIndex = 0;
}

std::pair<int8_t, int8_t> RandomMove::GenerateNextDirection()
{
    if (currentIndex >= directions.size()) {
        ShuffleDirections();
    }
    return directions[currentIndex++];
}