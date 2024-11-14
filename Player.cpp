#include "Player.h"

Player::Player(std::string name, Weapon weapon, std::pair<int, int>position) :
	name(name), weapon(weapon), position(position), points(0), lifes(3){}

void Player::Login()
{
	std::cout << "Player" << name << "logged in" << std::endl;
}

void Player::Movement()
{
	position.first += 1;
	position.second += 1;
	std::cout << "Player" << name << "moved to position(" << position.first << ", " << position.second << ")" << std::endl;
}

void Player::Shoot() 
{
	weapon.Shoot();
	std::cout << "Player" << name << "shoot with weapon\n"; 
}

void Player::ResetPosition()
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

int Player::GetLifes() const
{
	return lifes;
}

int Player::GetPoints() const
{
	return points;
}

std::pair<int, int> Player::GetPosition() const
{
	return position;
}

const std::string Player::GetName()
{
	return name;
}

int Player::GetScore()  
{
	return score;
}

void Player::SetScore(int newScore) 
{
	score = newScore;
}

void Player::SetPoints(int newPoints)
{
	points = newPoints;
}

bool Player::IsSpeedBoostUsed() const 
{
	return speedBoostUsed;
}

void Player::SetSpeedBoostUsed(bool used) 
{
	speedBoostUsed = used;
}

Weapon Player::GetWeapon()
{
	return weapon; 
}

