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
    void generateMap(int numPlayers);
    bool isPositionFree(std::pair<int,int> position);
    std::vector<Wall>& getWalls();
    std::vector<Bomb>& getBombs();
    std::pair<int, int> getSize();
};