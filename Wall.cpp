#include "Wall.h"

Wall::Wall(std::pair<int, int> position, WallType type, int durability, bool destructible, Bomb* bomb=nullptr)
{
    this->position = position;
    this->type = type;
    this->durability = durability;
    this->destructible = destructible;
    this->bomb = bomb;
}

std::pair<int, int> Wall::GetPosition()
{
    return position;
}

WallType Wall::GetType() 
{
    return type;
}

int Wall::GetDurability()
{
    return durability;
}

bool Wall::GetDestructible()
{
    return destructible;
}

void Wall::ReduceDurability()
{
    if (IsDestructible() == false)
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

bool Wall::IsDestructible()
{
    return type == WallType::Destructible;
}

void Wall::Destroy()
{
    if (IsDestructible() == true)
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