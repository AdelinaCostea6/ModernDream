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
};

