#include "Player.h"
#include <conio.h>

Player::Player(std::string name, std::shared_ptr<class Weapon> weapon, std::pair<int, int> position)
	: name(name), weapon(std::move(weapon)), position(position), initialPosition(position), points(0), lifes(3),score(0) {
}
void Player::Login()
{
	std::cout << "Player :" << name << "logged in" << std::endl;
}

void Player::Movement()
{
	if (_kbhit())
	{
		char key = _getch();

		switch (key)
		{
			case 'w':
				position.second -= 1;
				break;

			case 's':
				position.second += 1;
				break;

			case 'a':
				position.first -= 1;
				break;

			case 'd':
				position.first += 1;
				break;

			default:
				std::cout << "WRONG! USE W, A, S, D FOR MOVEMENT.\n";
				break;
		}
		std::cout << "Player " << name << " moved to position(" << position.first << ", " << position.second << ")" << std::endl;
	}
	
}

void Player::Shoot() 
{
	weapon->Shoot(); 
	std::cout << "Player :" << name << "shoot with weapon\n"; 
}

void Player::ResetPosition()
{
	position = initialPosition;
}

void Player::hit()
{
	lifes--;
	std::cout << "Player :" << name << "Was hit and lost a life.Lifes remaining:" << lifes << std::endl;
	if (lifes > 0)
	{
		ResetPosition();
		std::cout << "Position was reseted" << std::endl;
	}
	else
	{
		std::cout << "Game over for player" << name << "No more lifes " << std::endl;

	}
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

const std::string& Player::GetName()
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

Weapon& Player::GetWeapon() 
{ 
	return *weapon; 
}

