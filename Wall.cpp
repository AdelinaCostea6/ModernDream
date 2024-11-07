#include "Wall.h"

Wall::Wall(std::pair<int, int> position, int type, int durability, bool destructible)
{
    this.position = position;
    this.type = type;
    this.durability = durability;
    this.destructible = destructible;
}

std::pair<int, int> getPosition()
{
    return position;
}

int getType()
{
    return type;
}

int getDurability()
{
    return durability;
}

bool getDestructible()
{
    return destructible;
}

void reduceDurability(int amount)
{
    if (this.destructible)
    {
        this.durability = this.durablility - amount;
    }
    if (this.durablility < 0)
    {
        this.durability = 0;
    }
}