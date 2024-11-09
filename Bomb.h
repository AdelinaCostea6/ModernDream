#pragma once
#include <iostream>
#include <vector>
#include "Player.h"
//#include "Wall.h"

using Coordinates = std::pair<int, int>;
class Bomb
{
private:
	static const int radius = 10;
	Coordinates position;
	bool isActive;
public:
	Bomb(Coordinates position);
	Coordinates getPosition() const;
	bool getStatus() const;
	//void triggerBomb(int width, int height, std::vector<Player*> players, std::vector<Wall*> walls);
};
