#pragma once

#ifndef POINT_H
#include "Point.h"
#endif 

#ifndef GAMEMAP_H
#include "GameMap.h"
#endif 

class Base
{
	Point m_position;//The position of the base, which is at the middle of the last row.
	bool m_destroyed;//The base is destroyed or not.
	GameMap gameMap;
public:
	//Constructor.
	Base();

	//The base is destroyed.
    void Destroyed();

	//Returns the state of the map.
	bool IsDestroyed();
};

