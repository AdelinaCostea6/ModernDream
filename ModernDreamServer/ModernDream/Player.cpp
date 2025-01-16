#include "Player.h"
#include <conio.h>
#include "Game.h"

Player::Player(const std::string& name, std::unique_ptr<Weapon> weapon, std::pair<int, int> position)
	: name(name), weapon(std::move(weapon)), position(position), initialPosition(position){
}
 char Player::GetDirection() const
{
	return direction;
}
void Player::Login()
{
	std::cout << "Player :" << name << "logged in" << std::endl;
}

//void Player::Movement(Map& mapMatrix)
//{
//    if (_kbhit()) 
//    {
//        char key = _getch();
//        int newX = position.first;
//        int newY = position.second;
//
//        switch (key)
//        {
//        case 'w':
//            newX -= 1; 
//            break;
//        case 's':
//            newX += 1; 
//            break;
//        case 'a':
//            newY -= 1; 
//            break;
//        case 'd':
//            newY += 1; 
//            break;
//        default:
//            std::cout << "WRONG! USE W, A, S, D FOR MOVEMENT.\n";
//            return;
//        }
//
//        if (newX >= 0 && newX < mapMatrix.GetHeight() &&
//            newY >= 0 && newY < mapMatrix.GetWidth())
//        {
//            if (mapMatrix.IsMovable(newX, newY))
//            {
//                position.first = newX;
//                position.second = newY;
//                std::cout << "Player " << name << " moved to position("
//                    << position.first << ", " << position.second << ")" << std::endl;
//            }
//            else
//            {
//                std::cout << "Cannot move to wall or occupied space!\n";
//            }
//        }
//        else
//        {
//            std::cout << "Cannot move outside map boundaries!\n";
//        }
//    }
//}

//void Player::Movement(const Map& mapMatrix, char direction)
//{
//    int newX = position.first;
//    int newY = position.second;
//
//    switch (direction)
//    {
//    case 'w':  // Up
//        newX -= 1;
//        break;
//    case 's':  // Down
//        newX += 1;
//        break;
//    case 'a':  // Left
//        newY -= 1;
//        break;
//    case 'd':  // Right
//        newY += 1;
//        break;
//    default:
//        std::cout << "Invalid direction! Use W, A, S, D.\n";
//        return;
//    }
//
//    // Verifică dacă poziția este validă
//    if (newX >= 0 && newX < mapMatrix.GetHeight() &&
//        newY >= 0 && newY < mapMatrix.GetWidth())
//    {
//        if (mapMatrix.IsMovable(newX, newY))
//        {
//            position.first = newX;
//            position.second = newY;
//            std::cout << "Player " << name << " moved to position ("
//                << position.first << ", " << position.second << ")\n";
//        }
//        else
//        {
//            std::cout << "Cannot move to wall or occupied space!\n";
//        }
//    }
//    else
//    {
//        std::cout << "Cannot move outside map boundaries!\n";
//    }
//}

void Player::Movement(const Map& mapMatrix, char direction) {
	int newX = position.first;
	int newY = position.second;
	this->direction = direction;
	switch (direction) {
	case 'w': newX -= 1; break;  // Sus
	case 's': newX += 1; break;  // Jos
	case 'a': newY -= 1; break;  // Stânga
	case 'd': newY += 1; break;  // Dreapta
	}

	if (newX >= 0 && newX < mapMatrix.GetHeight()&&newY >= 0 && newY < mapMatrix.GetWidth())
		    {
		        if (mapMatrix.IsMovable(newX, newY))
		        {
		            position.first = newX;
		            position.second = newY;
		            std::cout << "Player " << name << " moved to position ("
		                << position.first << ", " << position.second << ")\n";
		        }
		        else
		        {
		            std::cout << "Cannot move to wall or occupied space!\n";
		        }
		    }
		    else
		    {
		        std::cout << "Cannot move outside map boundaries!\n";
		    }
}



//void Player::Shoot() 
//{
//	weapon->Shoot(); 
//	std::cout << "Player :" << name << "shoot with weapon\n"; 
//}

void Player::Shoot(Game& game) {
	if (weapon->CanShoot()) {
		game.ShootBullet(*this);  // Trimite cererea către `Game`
		weapon->Shoot();  // Reduce cooldown-ul
	}
}

void Player::ResetPosition()
{
	position = initialPosition;
}

void Player::Hit()
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

//std::pair<int, int> Player::GetPosition() const
//{
//	return position;
//}
std::pair<int, int> Player::GetPosition() const {
	if (!this) {
		std::cerr << "GetPosition called on null Player object!" << std::endl;
		throw std::runtime_error("Null Player object");
	}
	return position;
}


const std::string& Player::GetName() const
{
	return name;
}

int Player::GetScore() const
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

void Player::SetPosition(std::pair<int, int> pos) {
	position = pos; 
}
