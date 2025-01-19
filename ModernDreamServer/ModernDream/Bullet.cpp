#include "Bullet.h"


Bullet::Bullet(const Coordinates& position, char direction) : position(position), direction(direction) {}

void Bullet::SetIsInactive()
{
	isActive = false;
}
void Bullet::SetIsActive()
{
	isActive = true;
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

bool Bullet::GetIsActive() const
{
	return isActive;
}

void Bullet::SetDoubleSpeed()
{
	speed *= 2;
}



void Bullet::Movement(int maxHeight, int maxWidth) {
	if (isActive == true)
	{
		switch (direction) {
		case 'w': position.first -= 1; break; 
		case 's': position.first += 1; break;  
		case 'a': position.second -= 1; break; 
		case 'd': position.second += 1; break; 
		}
	}

	std::cout << "Noua directie: " << position.first << " " << position.second << "\n";


	if (position.first < 0 || position.second < 0 || position.first >= maxHeight || position.second >= maxWidth) {
		isActive = false;  
		std::cout << "Bullet-ul a ieșit din hartă și a fost dezactivat.\n";
	}
}


bool Bullet::CheckCollisionWithPlayers(std::array<std::unique_ptr<Player>, 4>& players)
{
	if (!isActive)
		return false;

	for (const auto& player : players) {
		if (!player) {
			continue;
		}
	}

	return std::any_of(players.begin(), players.end(), [this](const auto& player) {
		if (player->GetPosition() == position)
		{
			player->Hit();
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


void Bullet::CheckCollisionwithBullets(std::vector<std::unique_ptr<Bullet>>& bullets) {
	if (bullets.size() <= 1) return;

	for (size_t i = 0; i < bullets.size(); ++i) {
		for (size_t j = i + 1; j < bullets.size(); ++j) {
			if (bullets[i]->GetIsActive() && bullets[j]->GetIsActive() &&
				bullets[i]->GetPosition() == bullets[j]->GetPosition()) {
				bullets[i]->SetIsInactive();
				bullets[j]->SetIsInactive();
			}
		}
	}

	bullets.erase(
		std::remove_if(bullets.begin(), bullets.end(),
			[](const std::unique_ptr<Bullet>& bullet) { return !bullet->GetIsActive(); }),
		bullets.end());
}


char Bullet::GetDirection() const
{
	return direction;
}
