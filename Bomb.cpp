#include "Bomb.h"


Bomb::Bomb(Coordinates position)
	:position(position), isActive(true) {}

Coordinates Bomb::getPosition() const
{
	return position;
}

bool Bomb::getStatus() const
{
	return isActive;
}


