#pragma once
#include <iostream>
#include <vector>
#include "Player.h"

using Coordinates = std::pair<int, int>;
class Bomb
{
private:
	static const int radius = 10;
	Coordinates position;
	bool isActive;
	
public:
	Bomb(Coordinates position);
	Coordinates GetPosition() const;
	bool GetStatus() const;
	bool SetStatus(bool status);


};
