#pragma once
#include <iostream>
#include "Player.h"
#include <vector>
#include <string>
#include "Bullet.h"
class Weapon
{
private:
	float speed;
	float waitingTime;
	float lastShot;

public:
	Weapon() = default;
	Weapon(float speed);
	void Shoot(/*std::vector<Bullet*>& playerBullets*/);
	bool CanShoot();
	void UpgradeWaitingTime(float reduction);
	void UpgradeSpeed(float increase);
	float GetSpeed();
	float GetWaitingTime();
	float GetLastShot();
};
