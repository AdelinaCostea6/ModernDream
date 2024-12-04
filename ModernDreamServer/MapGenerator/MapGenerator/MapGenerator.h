/*
#pragma once

#ifdef MAPGENERATOR_EXPORTS
#define MAPGEN_API __declspec(dllexport)
#else
#define MAPGEN_API __declspec(dllimport)
#endif

#include "C:/Users/d/OneDrive/Desktop/ModernDream/ModernDreamServer/ModernDream/Map.h"

#include <vector>
#include <utility>

enum MapTile
{
    PlayerPosition = 0,
    FreeSpace = 1,
    DestructibleWall = 2,
    DestructibleWallWithBomb = 3,
    NonDestructibleWall = 4
};

class MAPGEN_API MapGenerator {
public:
    MapGenerator() = default;

    Map GenerateMap(int width, int height, int numPlayers);

private:
    void InitializeMapMatrix(std::vector<std::vector<int>>& mapMatrix, int width, int height);
    void GenerateClusters(std::vector<std::vector<int>>& mapMatrix, std::vector<Wall>& walls, int width, int height);
    void PlaceConnectorWalls(std::vector<std::vector<int>>& mapMatrix, std::vector<Wall>& walls, int width, int height);
    void SetPlayerStartPositions(std::vector<std::vector<int>>& mapMatrix, std::vector<Wall>& walls, int width, int height, int numPlayers);
    void PlaceBombs(std::vector<std::vector<int>>& mapMatrix,  std::vector<Wall>& walls, std::vector<Bomb>& bombs);
    void DisplayMap();
};
*/
#pragma once

#ifdef MAPGENERATOR_EXPORTS
#define MAPGEN_API __declspec(dllexport)
#else
#define MAPGEN_API __declspec(dllimport)
#endif

#include "C:/Users/d/OneDrive/Desktop/ModernDream/ModernDreamServer/ModernDream/Map.h"

//#include "../ModernDream/Map.h"
#include <vector>
#include <random>
#include <utility> 


enum MapTile
{
    PlayerPosition = 0,
    FreeSpace = 1,
    DestructibleWall = 2,
    DestructibleWallWithBomb = 3,
    NonDestructibleWall = 4
};

class MAPGEN_API MapGenerator {
public:
    MapGenerator();

    MapGenerator(std::pair<int, int> size);
    ~MapGenerator();
    Map GenerateMap(int numPlayers);

    const std::vector<std::vector<int>>& GetMapMatrix() const { return mapMatrix; }
    const std::vector<Wall>& GetWalls() const;
    const std::vector<Bomb>& GetBombs() const;

private:

    void InitializeMapMatrix();
    void GenerateClusters();
    void PlaceConnectorWalls();
    void SetPlayerStartPosition(int numPlayers);
    void PlaceBombs();
    void DisplayMap() const;


    std::pair<int, int> size; 
    std::vector<std::vector<int>> mapMatrix;
    std::vector<Wall> walls; 
    std::vector<Bomb> bombs; 
};
