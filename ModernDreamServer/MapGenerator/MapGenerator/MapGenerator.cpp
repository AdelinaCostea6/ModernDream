
#include "MapGenerator.h"
#include<random>
#include <algorithm>
#include <iostream>


MapGenerator::MapGenerator() {
    InitializeMapMatrix(); 
} 

void MapGenerator::GenerateMap(int numPlayers)
{
    InitializeMapMatrix();
    GenerateClusters();
    PlaceConnectorWalls();
    SetPlayerStartPosition(numPlayers);
    PlaceBombs();
    DisplayMap();
  
}

void MapGenerator::InitializeMapMatrix() {
    
    for (size_t i = 0; i < MapGenerator::kHeightG; ++i) {
        for (size_t j = 0; j < MapGenerator::kWidthG; ++j) {
            mapMatrix[i][j] = FreeSpace;
        }
    }
}

void MapGenerator::GenerateClusters() {
    std::random_device rd;
    std::mt19937 gen(rd());

    int numClusters = std::uniform_int_distribution<>(4, 8)(gen);
    std::uniform_int_distribution<> clusterSizeDist(3, 4);
    std::uniform_int_distribution<> wallTypeDist(0, 1);

    for (int cluster = 0; cluster < numClusters; cluster++) {
        int startY = std::uniform_int_distribution<>(2,MapGenerator::kHeightG  - 4)(gen);
        int startX = std::uniform_int_distribution<>(2, MapGenerator::kWidthG - 4)(gen);

        int clusterHeight = clusterSizeDist(gen);
        int clusterWidth = clusterSizeDist(gen);

        bool canPlace = true;
        for (int x = startX - 1; x <= startX + clusterWidth && canPlace; x++) {
            for (int y = startY - 1; y <= startY + clusterHeight && canPlace; y++) {
                if (x >= 0 && x < MapGenerator::kHeightG && y >= 0 && y < MapGenerator::kWidthG) {
                    if (mapMatrix[x][y] != FreeSpace) {
                        canPlace = false;
                    }
                }
            }
        }

        if (canPlace) {
            bool isHollow = std::uniform_int_distribution<>(0, 1)(gen);
            for (int x = startX; x < startX + clusterWidth && x < MapGenerator::kHeightG - 1; x++) {
                for (int y = startY; y < startY + clusterHeight && y < MapGenerator::kWidthG - 1; y++) {
                    if (!isHollow || x == startX || x == startX + clusterWidth - 1 ||
                        y == startY || y == startY + clusterHeight - 1) {
                        mapMatrix[x][y] = DestructibleWall;
                        bool isDestructible = (wallTypeDist(gen) == 1);
                        wallPositions.emplace_back(std::make_pair(x, y)); 
                        wallDurabilities.push_back(1); 
                        wallDestructibleFlags.push_back(isDestructible); 
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

    for (int x = 1; x < MapGenerator::kHeightG - 1; x++) {
        for (int y = 1; y < MapGenerator::kWidthG - 1; y++) {
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
                    wallPositions.emplace_back(std::make_pair(x, y));
                    wallDurabilities.push_back(1); 
                    wallDestructibleFlags.push_back(isDestructible);
                }
            }
        }
    }
}

void MapGenerator::SetPlayerStartPosition(int numPlayers) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::pair<int, int>> startPositions = {
        {0, 0}, {MapGenerator::kHeightG - 1, 0},
        {0, MapGenerator::kWidthG - 1}, {MapGenerator::kHeightG - 1, MapGenerator::kWidthG - 1}
    };
    std::shuffle(startPositions.begin(), startPositions.end(), gen);

    for (int i = 0; i < numPlayers; i++) {
        int px = startPositions[i].first;
        int py = startPositions[i].second;
        mapMatrix[px][py] = PlayerPosition;
    }
}

void MapGenerator::PlaceBombs() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> bombCountDist(0, 3);
    int bombCount = bombCountDist(gen);

    std::vector<size_t> eligibleWalls;
    for (size_t i = 0; i < wallPositions.size(); i++) {
        if (wallDestructibleFlags[i]) {
            eligibleWalls.push_back(i);
        }
    }

    if (!eligibleWalls.empty()) {
        std::shuffle(eligibleWalls.begin(), eligibleWalls.end(), gen);
        int actualBombCount = std::min(bombCount, static_cast<int>(eligibleWalls.size()));

        for (int i = 0; i < actualBombCount; i++) {
            size_t wallIndex = eligibleWalls[i];
            auto position = wallPositions[wallIndex];
            bombPositions.emplace_back(position);
            mapMatrix[position.first][position.second] = DestructibleWallWithBomb;
            bombStatuses.push_back(true);  
        }
    }
}

void MapGenerator::DisplayMap() const {
    std::cout << "Map generated with size (" << MapGenerator::kHeightG << ", " << MapGenerator::kWidthG << ")." << std::endl;
    for (const auto& row : mapMatrix) {
        for (int cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}
std::vector<std::pair<int, int>> MapGenerator::GetWallPositions() const {
    return wallPositions;
}

std::vector<int> MapGenerator::GetWallDurabilities() const {
    return wallDurabilities;
}

std::vector<bool> MapGenerator::GetWallDestructibleFlags() const {
    return wallDestructibleFlags;
}

std::vector<std::pair<int, int>> MapGenerator::GetBombPositions() const {
    return bombPositions;
}

std::vector<bool> MapGenerator::GetBombStatuses() const {
    return bombStatuses;
}
size_t MapGenerator::GetHeightG() const
{
    return kHeightG;
}
size_t MapGenerator::GetWidthG() const
{
    return kWidthG; 
}
MapGenerator::~MapGenerator() {}
