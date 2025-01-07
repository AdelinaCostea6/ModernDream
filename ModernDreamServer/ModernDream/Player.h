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
	std::unique_ptr<Weapon> weapon;
	int points{ 0 };
	int lifes{ 3 };
	std::pair<int, int> position{ 0, 0 };
	std::pair<int, int> initialPosition{ 0, 0 };
	int score{ 0 };
	bool speedBoostUsed{ false };
public:
    Player() = default;
    Player(const std::string &name, std::unique_ptr<Weapon> weapon, std::pair<int, int> position);

    void Login();
   // void Movement(Map& mapMatrix);
    void Shoot();
    void ResetPosition();
    void Hit();

    int GetLifes() const;
    int GetPoints() const;
    std::pair<int, int> GetPosition() const;
    const std::string& GetName() const;
    int GetScore() const;
    void Movement(const Map& mapMatrix, char direction);
    Weapon& GetWeapon();
    void SetScore(int newScore);
    void SetPoints(int newPoints);
    bool IsSpeedBoostUsed() const;
    void SetSpeedBoostUsed(bool used);
    void SetPosition(std::pair<int, int> pos);
    Player(const Player&) = delete;  
    Player& operator=(const Player&) = delete;
    Player(Player&&) = default;     
    Player& operator=(Player&&) = default;
    ~Player() = default;


};

