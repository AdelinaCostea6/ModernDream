#pragma once
#include <iostream>
#include <chrono>
#include "Player.h"
#include <vector>
#include <string>
#include "Bullet.h"
class Weapon
{
private:
	float speed;
	float waitingTime;
	std::chrono::steady_clock::time_point lastShot;

public:
	Weapon() noexcept = default;
	Weapon(float speed) noexcept;
	void Shoot(/*std::vector<Bullet*>& playerBullets*/);
	bool CanShoot() noexcept;
	void UpgradeWaitingTime(float reduction) noexcept;
	void UpgradeSpeed(float increase) noexcept;
	float GetSpeed() const noexcept;
	float GetWaitingTime() const noexcept;
	float GetLastShot() const noexcept;
};
