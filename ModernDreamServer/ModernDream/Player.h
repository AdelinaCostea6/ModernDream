#pragma once
#include <iostream>
#include <string>
#include <utility>
#include "Weapon.h"
#include "Map.h" 
class Weapon;
class Player
{
private:
	std::string name;
	std::shared_ptr<Weapon> weapon;
	int points{ 0 };
	int lifes{ 3 };
	std::pair<int, int> position{ 0, 0 };
	std::pair<int, int> initialPosition{ 0, 0 };
	int score{ 0 };
	bool speedBoostUsed{ false };
public:
    Player(std::string name, std::shared_ptr<Weapon> weapon, std::pair<int, int> position);

    void Login();
    void Movement(Map& mapMatrix);
    void Shoot();
    void ResetPosition();
    void Hit();

    int GetLifes() const;
    int GetPoints() const;
    std::pair<int, int> GetPosition() const;
    const std::string& GetName() const;
    int GetScore() const;

    Weapon& GetWeapon();
    void SetScore(int newScore);
    void SetPoints(int newPoints);
    bool IsSpeedBoostUsed() const;
    void SetSpeedBoostUsed(bool used);


};

