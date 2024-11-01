#include "Player.h"

Player::Player(std::string name, Weapon *weapon, std::pair<int,int>position)
{
	this->name = name;
	this->position.first = position.first;
	this->position.second = position.second;
	this->points = 0;
	this->lives = 3;
	this->weapon = weapon;


}
