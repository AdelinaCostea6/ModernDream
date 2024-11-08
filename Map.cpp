#include "Map.h"
 

std::vector<Wall>& Map::getWalls()
{
    return walls;
}

std::vector<Bomb>& Map::getBombs()
{
    return bombs; 
}

std::pair<int, int> Map::getSize()
{
    return size;
}
