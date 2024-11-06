#include "Player.h"

Player::Player(std::string name, Weapon* weapon, std::pair<int, int>position) :
	name(name), weapon(weapon), position(position), points(0), lifes(3){}

void Player::login()
{
	std::cout << "Player" << name << "logged in" << std::endl;
}

void Player::movement()
{
	position.first += 1;
	position.second += 1;
	std::cout << "Player" << name << "moved to position(" << position.first << ", " << position.second << ")" << std::endl;
}

void Player::shoot()
{
	weapon->shoot();
	std::cout << "Player" << name << "shoot with weapon:" << weapon;
}

void Player::resetPosition()
{
	if (lifes > 0)
	{
		lifes--;
		position = { 0,0 };
		std::cout << "Player" << name << "Was hit and lost a life.Lifes remaining:" << lifes << std::endl;
		std::cout << "Position was reseted" << std::endl;
	}
	else
		std::cout << "Game over for player" << name << "No more lifes " << std::endl;
}

