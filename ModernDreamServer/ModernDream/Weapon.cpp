#include "Weapon.h"
#include <chrono>

Weapon::Weapon(float speed) : speed(speed), waitingTime(4.0f), lastShot(std::chrono::steady_clock::now())
{
}

void Weapon::Shoot(/*std::vector<Bullet*>& playerBullets*/)
{
	if (CanShoot())
	{
		lastShot = std::chrono::steady_clock::now();
		// playerBullets.emplace_back(new Bullet(position));
	}
}

bool Weapon::CanShoot() const noexcept
{
	using namespace std::chrono;
	auto now = steady_clock::now();
	auto durationSinceLastShot = duration_cast<seconds>(now - lastShot).count();
	return durationSinceLastShot >= waitingTime;
}

void Weapon::UpgradeWaitingTime(float reduction) noexcept
{
	waitingTime -= reduction;
	if (waitingTime < 1.0f)
	{
		waitingTime = 1.0f;
	}
}
void Weapon::UpgradeSpeed(float increase) noexcept
{
	speed += increase;
}

float Weapon::GetSpeed() const noexcept
{
	return speed;
}

float Weapon::GetWaitingTime() const noexcept
{
	return waitingTime;
}

float Weapon::GetLastShot() const noexcept
{
	using namespace std::chrono;
	auto durationSinceLastShot = duration_cast<seconds>(steady_clock::now() - lastShot).count();
	return static_cast<float>(durationSinceLastShot);
}
