#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <utility>
#include "Bomb.h"
#include "Wall.h"

class Map
{
private:
    std::pair<int, int>size;
    std::vector<Wall> walls;
    std::vector<Bomb> bombs;
public:
    Map(std::pair<int, int> size) :size(size) {}
    void GenerateMap(int numPlayers);
    bool IsPositionFree(std::pair<int,int> position);
    std::vector<Wall>& GetWalls();
    std::vector<Bomb>& GetBombs();
    std::pair<int, int> GetSize();
};