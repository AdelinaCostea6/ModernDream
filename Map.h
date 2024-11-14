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
    std::vector<std::vector<int>> mapMatrix;
public:
    Map();
    Map(std::pair<int, int> size) :size(size) {}
    void GenerateMap(int numPlayers);
    bool IsPositionFree(std::pair<int,int> position);
    std::vector<Wall>& GetWalls();
    std::vector<Bomb>& GetBombs();
    std::pair<int, int> GetSize();
    Wall* GetWallAt(int x, int y);
    Bomb* GetBombAt(int x, int y);
    void SetFreePosition(int x, int y);
};