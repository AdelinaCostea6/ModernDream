#include "Wall.h"

Wall::Wall(std::pair<int, int> position, WallType type, int durability, bool destructible, Bomb* bomb)
    : position(position), type(type), durability(durability), destructible(destructible), bomb(bomb) {}


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
    std::cout << "The wall at (" << position.first << " , " << position.second << ")" << "was destroyed\n";
}