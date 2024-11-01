#pragma once
#include<iostream>
class Bullet
{
private:
	std::pair<int, int>startPosition;
	std::pair<int, int>endPosition;
	int direction;
	float speed;
public:
	Bullet();
	Bullet(std::pair<int, int> startPos, std::pair<int, int> endPos, int directionBullet, int speedBullet);
};

