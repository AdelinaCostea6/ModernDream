#include "Player.h"

Player::Player(std::string name, Weapon* weapon, std::pair<int, int>position) :
	name(name), weapon(weapon), position(position), points(0), lives(3){}

void Player::login()
{
	std::cout << "Player" << name << "logged in" << std::endl;
}

