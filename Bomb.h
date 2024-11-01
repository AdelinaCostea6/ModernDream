#pragma once
#include <iostream>

class Bomb
{
private:
	int radius;
	std::pair<int, int>position;

public:
	Bomb(int x, int y);
};

