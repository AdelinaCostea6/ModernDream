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

//void Bullet::Movement()
//{
//	/*position.first += 1;
//	position.second += 1;*/
//	switch (direction) {
//	case 'w': position.first -= 1; break;  
//	case 's': position.first += 1; break;  
//	case 'a': position.second -= 1; break; 
//	case 'd': position.second += 1; break;  
//	}
//
//}

void Bullet::Movement(int maxHeight, int maxWidth) {
	if (isActive == true)
	{
		switch (direction) {
		case 'w': position.first -= 1; break;  // Sus
		case 's': position.first += 1; break;  // Jos
		case 'a': position.second -= 1; break; // Stânga
		case 'd': position.second += 1; break; // Dreapta
		}
	}

	std::cout << "Noua directie: " << position.first << " " << position.second << "\n";

	// Verificare limite hartă
	if (position.first < 0 || position.second < 0 || position.first >= maxHeight || position.second >= maxWidth) {
		isActive = false;  // Dezactivează bullet-ul dacă iese din hartă
		std::cout << "Bullet-ul a ieșit din hartă și a fost dezactivat.\n";
	}
}


bool Bullet::CheckCollisionWithPlayers(std::array<std::unique_ptr<Player>, 4>& players)
{
	if (!isActive)
		return false;

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

//void Bullet::CheckCollisionwithBullets(std::vector<std::unique_ptr<Bullet>>& bullets)
//{
//	if (bullets.size() <= 1)
//		return;
//
//	for (auto it1 = bullets.begin(); it1 != bullets.end(); ++it1)
//	{
//		for (auto it2 = it1 + 1; it2 != bullets.end(); ++it2)
//		{
//			if ((*it1)->isActive && (*it2)->isActive && (*it1)->GetPosition() == (*it2)->GetPosition())
//			{
//				std::cout << "Two bullets collided\n";
//				(*it1)->SetIsInactive();
//				(*it2)->SetIsInactive();
//			}
//		}
//	}
//}
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
