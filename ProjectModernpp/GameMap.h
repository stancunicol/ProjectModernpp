#pragma once
#include <cstdint>
class GameMap
{
public:
    uint32_t width, height;

private:
    GameMap(int w, int h) : width(w), height(h) { }
};

