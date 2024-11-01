#pragma once
#include <iostream>
class Wall
{
private:
	std::pair<int, int>position;
	int type; //enum
	int durability;
	bool destructible;
public:
	Wall();
};

