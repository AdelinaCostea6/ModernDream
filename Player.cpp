#include "Player.h"

Player::Player(std::string name, Weapon* weapon, std::pair<int, int>position) :
	name(name), weapon(weapon), position(position), points(0), lives(3){}

void Player::login()
{
	std::cout << "Player" << name << "logged in" << std::endl;
}

void Player::movement(int dx, int dy)
{
	position.first += dx;
	position.second += dy;
	std::cout << "Player" << name << "moved to position(" << position.first << ", " << position.second << ")" << std::endl;
}

