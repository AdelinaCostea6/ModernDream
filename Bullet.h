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
	void SetIsInactive();
	void SetPosition(const Coordinates& pos);
	float GetSpeed() const;
	Coordinates GetPosition() const;
	bool GetIsInactive();
	void SetDoubleSpeed();
	void Movement();
	bool CheckCollisionWithPlayers(std::vector<Player*>& players);
	bool CheckCollisionwithWalls(std::vector<Wall*>& walls);
	void CheckCollisionwithBullets(std::vector<Bullet*>& bullets);
};

