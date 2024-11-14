#pragma once
#include <iostream>
#include <utility>
#include "Bomb.h"

enum class WallType 
{

    NonDestructible = 0,  
    Destructible = 1  
};
class Wall
{
private:
    std::pair<int,int> position;
    WallType type;
    int durability;
    bool destructible;
    Bomb* bomb;


public:
    Wall(std::pair<int, int> position, WallType type, int durability, bool destructible, Bomb* bomb=nullptr);
    WallType GetType();
    std::pair<int,int> GetPosition();
    int GetDurability();
    bool GetDestructible();
    void ReduceDurability();
    bool IsDestructible();
    void Destroy();
};