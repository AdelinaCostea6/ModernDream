#include "Bullet.h"

Bullet::Bullet() :position({ 0,0 }), speed(0.25), isActive(true) {}

Bullet::Bullet(Coordinates position)
	: position(position), speed(0.25), isActive(true){}

void Bullet::setIsInactive()
{
	isActive = false;
}

void Bullet::setPosition(const Coordinates& pos)
{
	position = pos;
}

float Bullet::getSpeed() const
{
	return speed;
}