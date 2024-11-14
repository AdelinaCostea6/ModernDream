#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include "Player.h"
#include "Wall.h"
#include "Bullet.h"

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
	void SetIsInactive();
	void SetPosition(const Coordinates& pos);
	float GetSpeed() const;
	Coordinates GetPosition() const;
	bool GetIsInactive();
	void SetDoubleSpeed();
	void Movement();
	bool CheckCollisionWithPlayers(std::vector<class Player>& players);
	bool CheckCollisionwithWalls(std::vector<class Wall>& walls);
	void CheckCollisionwithBullets(std::vector<class Bullet>& bullets);
};

