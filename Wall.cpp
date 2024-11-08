#include "Wall.h"

Wall::Wall(std::pair<int, int> position, WallType type, int durability, bool destructible, Bomb* bomb)
{
    this->position = position;
    this->type = type;
    this->durability = durability;
    this->destructible = destructible;
    this->bomb = bomb;
}

std::pair<int, int> Wall::getPosition()
{
    return position;
}

WallType Wall::getType() 
{
    return type;
}

int Wall::getDurability()
{
    return durability;
}

bool Wall::getDestructible()
{
    return destructible;
}

void Wall::reduceDurability()
{
    if (isDestructible() == false)
    {
        return;
    }
    if (destructible)
    {
        durability--;
    }
    if (durability < 0)
    {
        durability = 0;
    }
}

bool Wall::isDestructible()
{
    return type == WallType::Destructible;
}

void Wall::destroy()
{
    if (isDestructible() == true)
    {
        destructible = true;
        std::cout << "Wall at (" << position.first << ", " << position.second << ") was destroyed.\n";
        if (bomb) 
        {
            std::cout << "A bomb was triggered at (" << position.first << ", " << position.second << ")!\n";
            //bomb->triggerBomb()
        }

    }
}