#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <memory>
#include <algorithm>
#include <array>
#include <format>
#include "Player.h"
#include "Wall.h"
class Player;

using Coordinates = std::pair<int, int>;

class Bullet
{
private:
	Coordinates position{ 0, 0 }; 
	float speed{ 0.25f };          
	bool isActive{ true };

public:
	Bullet() = default;
	Bullet(Coordinates position);
	void SetIsInactive();
	void SetPosition(const Coordinates &pos);
	float GetSpeed() const;
	Coordinates GetPosition() const;
	bool GetIsInactive() const;
	void SetDoubleSpeed();
	void Movement();
	bool CheckCollisionWithPlayers(std::array<std::unique_ptr<Player>,4>&players);
	bool CheckCollisionwithWalls(std::vector<std::unique_ptr<Wall>> &walls);
	void CheckCollisionwithBullets(std::vector<std::unique_ptr<Bullet>> &bullets);
};
