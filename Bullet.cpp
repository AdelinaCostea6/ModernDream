#include "Bullet.h"

Bullet::Bullet() :position({ 0,0 }), speed(0.25), isActive(true) {}

Bullet::Bullet(Coordinates position)
	: position(position), speed(0.25), isActive(true){}

void Bullet::setIsInactive()
{
	isActive = false;
}

void Bullet::setPosition(const Coordinates& pos)
{
	position = pos;
}

float Bullet::getSpeed() const
{
	return speed;
}

Coordinates Bullet::getPosition() const
{
	return position;
}

void Bullet::setDoubleSpeed()
{
	speed *= 2;
}


void Bullet::movement()
{
	position.first += 1;
	position.second += 1;
	std::cout << "The bullet was moved to (" << position.first << ", " << position.second << ") " << std::endl;


}

bool Bullet::checkCollisionWithPlayers(std::vector<Player*>& players)
{
	if (!isActive) return false;

		for (auto& player : players)
		{
			if (player->getPosition() == position)
			{
				player->resetPosition();
				std::cout << "Player " << player->getName() << " has been shoot\\n";
				isActive = false;
				return true;
			}
		}
	return false;


}

bool Bullet::checkCollisionwithWalls(std::vector<Wall*>& walls)
{
	if (!isActive) return false;

		for (auto& wall : walls)
		{
			if (wall->getPosition() == position)
			{
				if (wall->isDestructible())
				{
					wall->destroy();
					std::cout << "The wall at (" << wall->getPosition().first <<" , "<< wall->getPosition().second <<") has been destroyed\\n";
					isActive = false;
					return true;
				}
			}
		}
	return false;


}

void Bullet::checkCollisionwithBullets(std::vector<Bullet*>& bullets)
{
	for (int i = 0; i < bullets.size() - 1; i++)
	{
		for (int j = i + 1; j < bullets.size(); j++)
		{
			if (bullets[i]->isActive && bullets[j]->isActive && bullets[i]->getPosition() == bullets[j]->getPosition())
			{
				std::cout << "Two bullets collided\n";
				bullets[i]->setIsInactive();
				bullets[j]->setIsInactive();
			}

		}
	}

}