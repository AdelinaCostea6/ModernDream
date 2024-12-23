#include "Weapon.h"

Weapon::Weapon(float speed) : speed(speed)
{
}

void Weapon::Shoot()
{
	if (CanShoot())
	{
		lastShot = (float)std::time(nullptr);
		// playerBullets.emplace_back(new Bullet(position));
	}
}

bool Weapon::CanShoot() const
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

float Weapon::GetSpeed() const
{
	return speed;
}

float Weapon::GetWaitingTime() const
{
	return waitingTime;
}

float Weapon::GetLastShot() const
{
	return lastShot;
}
