#include "Weapon.h"

Weapon::Weapon(float speed):speed(speed),waitingTime(4.0f),lastShot(0.0f)
{}

void Weapon::Shoot(/*std::vector<Bullet*>& playerBullets*/)
{
	if (CanShoot())
	{
		lastShot = (float)std::time(nullptr);
		//playerBullets.emplace_back(new Bullet(position));
	}
}

bool Weapon::CanShoot()
{
	return (lastShot + waitingTime) <= (float)std::time(nullptr);
}

void Weapon::UpgradeWaitingTime(float reduction)
{
	waitingTime -= reduction;
	if (waitingTime < 1.0f)
	{
		waitingTime = 1.0f;
	}
}
void Weapon::UpgradeSpeed(float increase)
{
	speed += increase;
}
