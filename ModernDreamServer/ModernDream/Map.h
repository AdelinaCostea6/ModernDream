#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <utility>
#include <array>
#include "Wall.h"
#include "../MapGenerator/MapGenerator/MapGenerator.h"
import bomb;


class Map
{
private:
    static const size_t kHeight = 15;
    static const size_t kWidth = 15;
    std::vector<std::unique_ptr<Wall>> walls;
    std::array<std::unique_ptr<Bomb>,3> bombs;
    std::array<std::array<int, kWidth>, kHeight> mapMatrix{};
public:
    Map();
    Map(MapGenerator& generator);
  
    bool IsPositionFree(std::pair<int,int> position);
    bool IsMovable(int x, int y);
    const std::vector<std::unique_ptr<Wall>>& GetWalls();
    const std::array<std::unique_ptr<Bomb>, 3>& GetBombs();
    /*std::pair<int, int> GetSize();*/
    size_t GetHeight();
    size_t GetWidth();
    Wall* GetWallAt(int x, int y);
    std::unique_ptr<Bomb>* GetBombAt(int x, int y); 
    void SetFreePosition(int x, int y);
    void SetWalls(const std::vector<std::unique_ptr<Wall>>& newWalls);
    void SetBombs(const std::array<std::unique_ptr<Bomb>, 3>& newBombs);  
};