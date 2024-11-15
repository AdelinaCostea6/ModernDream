#pragma once
#include <iostream>
#include <string>
#include <utility>
#include "Weapon.h"
class Player
{
private:
	std::string name; 
	std::shared_ptr<class Weapon> weapon; 
	int points;
	int lifes;
	std::pair<int, int> position;
	std::pair<int, int>initialPosition;
	int score;
	bool speedBoostUsed=false;
public:
	Player(std::string name, std::shared_ptr<class Weapon> weapon, std::pair<int, int> position);
	void Login();
	void Movement();
	void Shoot();
	void ResetPosition();
	void hit();

	int GetLifes()const;
	int GetPoints()const;
	std::pair<int, int>GetPosition()const;
	const std::string& GetName();
	int GetScore();
	Weapon& GetWeapon();

	void SetScore(int newScore);
	void SetPoints(int newPoints);
	bool IsSpeedBoostUsed()const;
	void SetSpeedBoostUsed(bool used);


};

