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
    /*static const size_t kHeight = 18;
    static const size_t kWidth = 30;*/
    size_t height;
    size_t width;

    std::vector<std::unique_ptr<Wall>> walls;
    std::array<std::unique_ptr<Bomb>, 3> bombs;
    std::vector<std::vector<int>> mapMatrix;
    

public:
    Map();
    Map(MapGenerator& generator);
    Map(size_t h, size_t w);

    bool IsPositionFree(std::pair<int, int> position) const;
    bool IsMovable(int x, int y) const;
    const std::vector<std::unique_ptr<Wall>>& GetWalls();
    const std::array<std::unique_ptr<Bomb>, 3>& GetBombs();
    /*std::pair<int, int> GetSize();*/
    size_t GetHeight() const;
    size_t GetWidth() const;
    Wall* GetWallAt(int x, int y);
    std::unique_ptr<Bomb>* GetBombAt(int x, int y);
    void SetFreePosition(int x, int y);
    void SetWalls(std::vector<std::unique_ptr<Wall>> newWalls);
    void SetBombs(const std::array<std::unique_ptr<Bomb>, 3>& newBombs);
    const std::vector<std::vector<int>>& GetMapMatrix() const;
    std::vector<std::pair<int, int>> GetPlayerStartPositions() const;
};