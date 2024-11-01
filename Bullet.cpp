#include "Bullet.h"

Bullet::Bullet() :startPosition({ 0,0 }), endPosition({0,0}),direction({0}),speed({0}) {}

Bullet::Bullet(std::pair<int, int> startPos, std::pair<int, int> endPos, int directionBullet, int speedBullet):
startPosition(startPos),endPosition(endPos),direction(directionBullet),speed(speedBullet){}
