#pragma once
#include<iostream>
#include  "Weapon.h"
class Player
{
private:
	std::string name; 
	Weapon *weapon; 
	int points;
	int lifes;
	std::pair<int, int> position;
public:
	Player(std::string name, Weapon *weapon, std::pair<int,int>position);
	void login();
	void movement();
	void shoot();
	void resetPosition();
	int getLifes()const;
	int getPoints()const;
	std::pair<int, int>getPosition()const;

};

