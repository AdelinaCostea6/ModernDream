#pragma once
#include<iostream>
#include <vector>
#include "Player.h"
#include "Wall.h"

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
	Coordinates getPosition() const;
	float setDoubleSpeed();
	void movement();
	bool checkCollisionWithPlayers(std::vector<Player*>& players);
	bool checkCollisionwithWalls(std::vector<Wall*>& walls);
	void checkCollisionwithBullets(std::vector<Bullet*>& bullets);
};

