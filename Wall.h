#pragma once
#include <iostream>
class Wall
{
private:
    std::pair<int, int> position;
    int type; // enum
    int durability;
    bool destructible;

public:
    Wall();
    std::pair<int, int> getPosition();
    int getType();
    int getDurability();
    bool getDestructible();
    void reduceDurability();
    bool isDestructible();
    void destroy();
};