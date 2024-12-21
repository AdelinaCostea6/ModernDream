#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include "Player.h"
#include "Wall.h"

using Coordinates = std::pair<int, int>;

class Bullet
{
private:
	Coordinates position{0, 0};
	float speed = 0.25f;
	bool isActive = true;

public:
	Bullet() noexcept = default;
	explicit Bullet(Coordinates position) noexcept;
	void SetIsInactive() noexcept;
	void SetPosition(const Coordinates &pos) noexcept;
	[[nodiscard]] float GetSpeed() const noexcept;
	[[nodiscard]] Coordinates GetPosition() const noexcept;
	[[nodiscard]] bool GetIsInactive() noexcept;
	void SetDoubleSpeed() noexcept;
	void Movement();
	bool CheckCollisionWithPlayers(std::vector<class Player> &players);
	bool CheckCollisionwithWalls(std::vector<Wall> &walls);
	void CheckCollisionwithBullets(std::vector<Bullet> &bullets);
};
