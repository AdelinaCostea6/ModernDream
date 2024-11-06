#pragma once
#include<iostream>
#include  "Weapon.h"
class Player
{
private:
	std::string name; 
	Weapon *weapon; 
	int points;
	int lives;
	std::pair<int, int> position;
public:
	Player(std::string name, Weapon *weapon, std::pair<int,int>position);
	void login();
	void movement(int dx,int dy);
};

