#include "Bullet.h"

Bullet::Bullet()
{
	this->direction = 0;
	this->position.first = 0;
	this->position.second = 0;
	this->speed = 0, 25;
}
Bullet::Bullet() :startPosition({ 0,0 }), endPosition({0,0}),direction({0}),speed({0}) {}

Bullet::Bullet(std::pair<int, int> startPos, std::pair<int, int> endPos, int directionBullet, int speedBullet):
startPosition(startPos),endPosition(endPos),direction(directionBullet),speed(speedBullet){}
