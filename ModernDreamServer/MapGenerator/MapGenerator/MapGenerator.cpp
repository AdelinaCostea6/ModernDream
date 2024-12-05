
#include "MapGenerator.h"
#include<random>
#include <algorithm>
#include <iostream>


MapGenerator::MapGenerator():size(10,10)
{
}
MapGenerator::MapGenerator(std::pair<int, int> size) : size(size) {
}

Map MapGenerator::GenerateMap(int numPlayers)
{
    InitializeMapMatrix();
    GenerateClusters();
    PlaceConnectorWalls();
    SetPlayerStartPosition(numPlayers);
    PlaceBombs();
    DisplayMap();

    
    Map generatedMap(size);
    generatedMap.SetWalls(walls);
    generatedMap.SetBombs(bombs); 

    return generatedMap;  
}

const std::vector<Wall>& MapGenerator::GetWalls() const
{
    return walls;
}

const std::vector<Bomb>& MapGenerator::GetBombs() const
{
    return bombs;
}

void MapGenerator::InitializeMapMatrix() {
    mapMatrix.resize(size.first, std::vector<int>(size.second, FreeSpace));
}

void MapGenerator::GenerateClusters() {
    std::random_device rd;
    std::mt19937 gen(rd());

    int numClusters = std::uniform_int_distribution<>(4, 8)(gen);
    std::uniform_int_distribution<> clusterSizeDist(3, 4);
    std::uniform_int_distribution<> wallTypeDist(0, 1);

    for (int cluster = 0; cluster < numClusters; cluster++) {
        int startY = std::uniform_int_distribution<>(2, size.first - 4)(gen);
        int startX = std::uniform_int_distribution<>(2, size.second - 4)(gen);

        int clusterHeight = clusterSizeDist(gen);
        int clusterWidth = clusterSizeDist(gen);

        bool canPlace = true;
        for (int x = startX - 1; x <= startX + clusterWidth && canPlace; x++) {
            for (int y = startY - 1; y <= startY + clusterHeight && canPlace; y++) {
                if (x >= 0 && x < size.first && y >= 0 && y < size.second) {
                    if (mapMatrix[x][y] != FreeSpace) {
                        canPlace = false;
                    }
                }
            }
        }

        if (canPlace) {
            bool isHollow = std::uniform_int_distribution<>(0, 1)(gen);
            for (int x = startX; x < startX + clusterWidth && x < size.first - 1; x++) {
                for (int y = startY; y < startY + clusterHeight && y < size.second - 1; y++) {
                    if (!isHollow || x == startX || x == startX + clusterWidth - 1 ||
                        y == startY || y == startY + clusterHeight - 1) {
                        mapMatrix[x][y] = DestructibleWall;
                        bool isDestructible = (wallTypeDist(gen) == 1);
                        WallType type = isDestructible ? WallType::Destructible : WallType::NonDestructible;
                        walls.emplace_back(std::make_pair(x, y), type, 1, isDestructible, nullptr);
                    }
                }
            }
        }
    }
}

void MapGenerator::PlaceConnectorWalls() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> connectorDist(0, 3);
    std::uniform_int_distribution<> wallTypeDist(0, 1);

    for (int x = 1; x < size.first - 1; x++) {
        for (int y = 1; y < size.second - 1; y++) {
            if (mapMatrix[x][y] == FreeSpace) {
                bool hasNearbyWalls = false;
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (mapMatrix[x + dx][y + dy] == DestructibleWall) {
                            hasNearbyWalls = true;
                            break;
                        }
                    }
                }
                if (hasNearbyWalls && connectorDist(gen) == 0) {
                    mapMatrix[y][x] = DestructibleWall;
                    bool isDestructible = (wallTypeDist(gen) == 1);
                    WallType type = isDestructible ? WallType::Destructible : WallType::NonDestructible;
                    walls.emplace_back(std::make_pair(y, x), type, 1, isDestructible, nullptr);
                }
            }
        }
    }
}

void MapGenerator::SetPlayerStartPosition(int numPlayers) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::pair<int, int>> startPositions = {
        {0, 0}, {size.first - 1, 0},
        {0, size.second - 1}, {size.first - 1, size.second - 1}
    };
    std::shuffle(startPositions.begin(), startPositions.end(), gen);

    for (int i = 0; i < numPlayers; i++) {
        int px = startPositions[i].first;
        int py = startPositions[i].second;
        mapMatrix[px][py] = PlayerPosition;

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int nx = px + dx;
                int ny = py + dy;
                if (nx >= 0 && nx < size.first && ny >= 0 && ny < size.second) {
                    if (mapMatrix[nx][ny] == DestructibleWall) {
                        mapMatrix[nx][ny] = FreeSpace;
                        walls.erase(
                            std::remove_if(walls.begin(), walls.end(),
                                [nx, ny](Wall& wall) {
                                    return wall.GetPosition() == std::make_pair(nx, ny);
                                }),
                            walls.end()
                        );
                    }
                }
            }
        }
    }
}

void MapGenerator::PlaceBombs() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> bombCountDist(0, 3);
    int bombCount = bombCountDist(gen);

    std::vector<size_t> eligibleWalls;
    for (size_t i = 0; i < walls.size(); i++) {
        if (walls[i].IsDestructible()) {
            eligibleWalls.push_back(i);
        }
    }

    if (!eligibleWalls.empty()) {
        std::shuffle(eligibleWalls.begin(), eligibleWalls.end(), gen);
        int actualBombCount = std::min(bombCount, static_cast<int>(eligibleWalls.size()));

        for (int i = 0; i < actualBombCount; i++) {
            size_t wallIndex = eligibleWalls[i];
            auto position = walls[wallIndex].GetPosition();
            bombs.emplace_back(position);
            mapMatrix[position.first][position.second] = DestructibleWallWithBomb;
        }
    }
}

void MapGenerator::DisplayMap() const {
    std::cout << "Map generated with size (" << size.first << ", " << size.second << ")." << std::endl;
    for (const auto& row : mapMatrix) {
        for (int cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

MapGenerator::~MapGenerator() { }
