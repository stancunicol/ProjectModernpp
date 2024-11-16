#include "Base.h"

Base::Base()
	: m_destroyed(false)
{
	uint8_t x = gameMap.GetWidth() / 2;
	uint8_t y = gameMap.GetHeight();
	m_position = Point(x, y);
}

void Base::Destroyed()
{
	m_destroyed = true;
}

bool Base::IsDestroyed()
{
	return m_destroyed;
}
