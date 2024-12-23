#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Bullet.h"
class Weapon
{
private:
	float speed;
	float waitingTime{ 4.0f };
	float lastShot{ 0.0f };

public:
	Weapon() = default;
	Weapon(float speed);
	void Shoot();
	bool CanShoot() const;
	void UpgradeWaitingTime(float reduction);
	void UpgradeSpeed(float increase);
	float GetSpeed() const;
	float GetWaitingTime() const;
	float GetLastShot() const;
};
