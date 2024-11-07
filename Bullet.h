#pragma once
#include<iostream>

using Coordinates = std::pair<int, int>;

class Bullet
{
private:
	Coordinates position;
	float speed;
	bool isActive;

public:
	Bullet();
	Bullet(Coordinates position);
	void setIsInactive();
	void setPosition(const Coordinates& pos);
	float getSpeed() const;
};

