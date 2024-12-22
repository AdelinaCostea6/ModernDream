#include "Bullet.h"

Bullet::Bullet() : position({0, 0}), speed(0.25), isActive(true) {}

Bullet::Bullet(Coordinates position) : position(position), speed(0.25), isActive(true) {}

void Bullet::SetIsInactive()
{
	isActive = false;
}

void Bullet::SetPosition(const Coordinates &pos)
{
	position = pos;
}

float Bullet::GetSpeed() const
{
	return speed;
}

Coordinates Bullet::GetPosition() const
{
	return position;
}

bool Bullet::GetIsInactive()
{
	return isActive;
}

void Bullet::SetDoubleSpeed()
{
	speed *= 2;
}

void Bullet::Movement()
{
	position.first += 1;
	position.second += 1;
	std::cout << "The bullet was moved to (" << position.first << ", " << position.second << ") " << std::endl;
}

bool Bullet::CheckCollisionWithPlayers(std::vector<Player> &players)
{
	if (!isActive)
		return false;

	for (auto &player : players)
	{
		if (player.GetPosition() == position)
		{
			player.ResetPosition();
			std::cout << "Player " << player.GetName() << " has been shoot\\n";
			isActive = false;
			return true;
		}
	}
	return false;
}

bool Bullet::CheckCollisionwithWalls(std::vector<Wall> &walls)
{
	if (!isActive)
		return false;

	for (auto &wall : walls)
	{
		if (wall.GetPosition() == position)
		{
			if (wall.IsDestructible())
			{
				wall.Destroy();
				std::cout << "The wall at (" << wall.GetPosition().first << " , " << wall.GetPosition().second << ") has been destroyed\\n";
				isActive = false;
				return true;
			}
		}
	}
	return false;
}

void Bullet::CheckCollisionwithBullets(std::vector<Bullet> &bullets)
{
	for (int i = 0; i < bullets.size() - 1; i++)
	{
		for (int j = i + 1; j < bullets.size(); j++)
		{
			if (bullets[i].isActive && bullets[j].isActive && bullets[i].GetPosition() == bullets[j].GetPosition())
			{
				std::cout << "Two bullets collided\n";
				bullets[i].SetIsInactive();
				bullets[j].SetIsInactive();
			}
		}
	}
}