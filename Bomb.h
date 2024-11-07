#pragma once
#include <iostream>
#include <vector>
#include "Player.h"
#include "Wall.h"
#include "Map.h"

class Bomb
{
private:
	int radius;
	std::pair<int, int>position;
public:
	Bomb(int x, int y);
};
