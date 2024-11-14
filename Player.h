#pragma once
#include<iostream>
#include  "Weapon.h"
class Player
{
private:
	std::string name; 
	Weapon weapon; 
	int points;
	int lifes;
	std::pair<int, int> position;
	int score;
	bool speedBoostUsed;
public:
	Player(std::string name, Weapon weapon, std::pair<int,int>position);
	void Login();
	void Movement();
	void Shoot();
	void ResetPosition();
	int GetLifes()const;
	int GetPoints()const;
	std::pair<int, int>GetPosition()const;
	const std::string GetName();
	int GetScore();
	void SetScore(int newScore);
	void SetPoints(int newPoints);
	bool IsSpeedBoostUsed()const;
	void SetSpeedBoostUsed(bool used);
	Weapon GetWeapon();


};

