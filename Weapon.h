#pragma once
#include <iostream>
#include "Player.h"
#include <vector>
#include "Bullet.h" 
class Weapon
{
private:
	float speed;
	float waitingTime;
	float lastShot;
public:
	Weapon(float speed);
	void Shoot(/*std::vector<Bullet*>& playerBullets*/);
	bool CanShoot();
	void UpgradeWaitingTime(float reduction);
	void UpgradeSpeed(float increase);
};

