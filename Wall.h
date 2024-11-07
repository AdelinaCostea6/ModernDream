#pragma once
#include <iostream>
#include "Bomb.h"

class Wall
{
private:
    std::pair<int, int> position;
    int type; // enum
    int durability;
    bool destructible;
    Bomb* bomb;


public:
    Wall(std::pair<int, int> position, int type, int durability, bool destructible, Bomb* bomb=nullptr);
    int getType();
    std::pair<int, int> getPosition();
    int getDurability();
    bool getDestructible();
    void reduceDurability(int amount);
    bool isDestructible();
    void destroy();
};