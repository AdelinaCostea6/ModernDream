#pragma once
#include <iostream>
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
    WallType getType();
    std::pair<int,int> getPosition();
    int getDurability();
    bool getDestructible();
    void reduceDurability();
    bool isDestructible();
    void destroy();
};