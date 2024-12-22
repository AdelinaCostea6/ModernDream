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

#include <vector>
#include <random>
#include <utility>
#include <array> 




class MAPGEN_API MapGenerator {
private:

    void InitializeMapMatrix();
    void GenerateClusters();
    void PlaceConnectorWalls();
    void SetPlayerStartPosition(int numPlayers);
    void PlaceBombs(); 

    /*std::pair<int, int> size;
    std::vector<std::vector<int>> mapMatrix;*/
    static const size_t kHeightG = 15;
    static const size_t kWidthG = 15;
    std::array<std::array<int, kWidthG>, kHeightG> mapMatrix;


    // Members to hold raw data for walls and bombs
    std::vector<std::pair<int, int>> wallPositions;
    std::vector<int> wallDurabilities;
    std::vector<bool> wallDestructibleFlags;

    std::vector<std::pair<int, int>> bombPositions;
    std::vector<bool> bombStatuses;
public:
    enum MapTile
    {
        PlayerPosition = 0,
        FreeSpace = 1,
        DestructibleWall = 2,
        DestructibleWallWithBomb = 3,
        NonDestructibleWall = 4
    };

    MapGenerator();
    //MapGenerator(std::pair<int, int> mapSize); 
    ~MapGenerator();

    void GenerateMap(int numPlayers);
    void DisplayMap() const;

    std::vector<std::pair<int, int>> GetWallPositions() const;
    std::vector<int> GetWallDurabilities() const;
    std::vector<bool> GetWallDestructibleFlags() const;

    std::vector<std::pair<int, int>> GetBombPositions() const;
    std::vector<bool> GetBombStatuses() const;

    size_t GetHeightG() const;

    // Get the width of the map
    size_t GetWidthG() const;

    // Get the map matrix
    const std::array<std::array<int, kWidthG>, kHeightG>& GetMapMatrix() const; 

};
