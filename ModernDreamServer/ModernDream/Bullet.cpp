#include "Bullet.h"


Bullet::Bullet(Coordinates position) : position(position) {}

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

bool Bullet::GetIsInactive() const
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

bool Bullet::CheckCollisionWithPlayers(std::array<std::unique_ptr<Player>, 4>& players)
{
	if (!isActive)
		return false;

	return std::any_of(players.begin(), players.end(), [this](const auto& player) {
		if (player->GetPosition() == position)
		{
			player->ResetPosition();
			std::cout << std::format("Player {} has been shot\n", player->GetName());
			isActive = false;
			return true;
		}
		return false;
		});
}

bool Bullet::CheckCollisionwithWalls(std::vector<std::unique_ptr<Wall>>& walls)
{
	if (!isActive)
		return false;

	return std::any_of(walls.begin(), walls.end(), [this](const auto& wall) {
		if (wall->GetPosition() == position)
		{
			if (wall->IsDestructible())
			{
				wall->Destroy();
				std::cout << std::format("The wall at ({}, {}) has been destroyed\n",
					wall->GetPosition().first, wall->GetPosition().second);
				isActive = false;
				return true;
			}
		}
		return false;
		});
}

void Bullet::CheckCollisionwithBullets(std::vector<std::unique_ptr<Bullet>>& bullets)
{
	if (bullets.size() <= 1)
		return;

	for (auto it1 = bullets.begin(); it1 != bullets.end(); ++it1)
	{
		for (auto it2 = it1 + 1; it2 != bullets.end(); ++it2)
		{
			if ((*it1)->isActive && (*it2)->isActive && (*it1)->GetPosition() == (*it2)->GetPosition())
			{
				std::cout << "Two bullets collided\n";
				(*it1)->SetIsInactive();
				(*it2)->SetIsInactive();
			}
		}
	}
}