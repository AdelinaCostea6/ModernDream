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


enum WallTypeG {
    NonDestructible = 0,
    Destructible = 1
};
class MAPGEN_API WallG {
public:
    WallG(std::pair<int, int> pos, WallTypeG type, int durability, bool destructible);

    std::pair<int, int> GetPosition() const;
    WallTypeG GetType() const;
    int GetDurability() const;
    bool IsDestructible() const;
    void ReduceDurability();
    //BombG* GetBomb() ;

private:
    std::pair<int, int> position;
    WallTypeG type;
    int durability;
    bool isDestructible;
    //BombG* bomb;
};

class MAPGEN_API BombG {
public:
    BombG(std::pair<int, int>);

    std::pair<int, int> GetPosition() const;
    bool GetStatus() const;
    void SetStatus(bool newStatus);

private:
    std::pair<int, int> position;
    bool status;
};

class MAPGEN_API MapG {
public:
    MapG(std::pair<int, int> mapSize);

    void SetWalls(const std::vector<WallG>& newWalls);
    void SetBombs(const std::vector<BombG>& newBombs);

    std::vector<WallG>& GetWalls();
    std::vector<BombG>& GetBombs();
    std::pair<int, int> GetSize() const;

private:
    std::pair<int, int> size;
    std::vector<WallG> walls;
    std::vector<BombG> bombs;
    std::vector<std::vector<int>> mapMatrix;
};
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
    MapGenerator(std::pair<int, int> mapSize);
    ~MapGenerator();

    MapG GenerateMap(int numPlayers);

    //const std::vector<std::vector<int>>& GetMapMatrix() const { return mapMatrix; }
    const std::vector<WallG>& GetWalls() const;
    const std::vector<BombG>& GetBombs() const;
    void DisplayMap() const;
private:

    void InitializeMapMatrix();
    void GenerateClusters();
    void PlaceConnectorWalls();
    void SetPlayerStartPosition(int numPlayers);
    void PlaceBombs();



    std::pair<int, int> size; 
    std::vector<std::vector<int>> mapMatrix;
    std::vector<WallG> walls; 
    std::vector<BombG> bombs; 
};
