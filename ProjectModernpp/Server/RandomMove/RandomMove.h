#pragma once
#include <vector>
#include <random>
#include <utility>
#include <algorithm>

class __declspec(dllexport) RandomMove
{
private:
    std::vector<std::pair<int8_t, int8_t>> directions{
        { -1, 0 }, //up
        { 1, 0 }, //down
        { 0, -1 }, //left
        { 0, 1 } //right
    };
    size_t currentIndex = 0;

public:
    RandomMove();

    void ShuffleDirections();

    std::pair<int8_t, int8_t> GenerateNextDirection();
};
