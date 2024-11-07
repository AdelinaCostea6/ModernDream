#include "Wall.h"

Wall::Wall(std::pair<int, int> position, int type, int durability, bool destructible, Bomb* bomb=nullptr)
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

int Wall::getType()
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

void Wall::reduceDurability(int amount)
{
    if (isDestructible() == false)
    {
        return;
    }
    if (destructible)
    {
        durability = durability - amount;
    }
    if (durability < 0)
    {
        durability = 0;
    }
}

bool Wall::isDestructible()
{
    if (type == 0 && !destructible) // destructibil
    {
        return true;
    }
    else // indestructibil
    {
        return false;
    }
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