#pragma once
#include <iostream>
#include <vector>
#include "Player.h"
#include "Wall.h"
#include "Map.h"

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
	void triggerBomb(int width, int height, const std::vector<Player*>& players, const std::vector<Wall*>& walls);
};
