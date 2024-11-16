#include "Map.h"


Map::Map():size({0,0})
{
}

void Map::GenerateMap(int numPlayers) {
    mapMatrix.resize(size.second, std::vector<int>(size.first, 1));

    std::random_device rd;
    std::mt19937 gen(rd());

    int numClusters = std::uniform_int_distribution<>(4, 8)(gen);
    std::uniform_int_distribution<> clusterSizeDist(3, 4);
    std::uniform_int_distribution<> wallTypeDist(0, 1);

    for (int cluster = 0; cluster < numClusters; cluster++)
    {
        int startX = std::uniform_int_distribution<>(2, size.first - 4)(gen);
        int startY = std::uniform_int_distribution<>(2, size.second - 4)(gen);

        int clusterWidth = clusterSizeDist(gen);
        int clusterHeight = clusterSizeDist(gen);

        bool canPlace = true;

        for (int y = startY - 1; y <= startY + clusterHeight && canPlace; y++) {
            for (int x = startX - 1; x <= startX + clusterWidth && canPlace; x++) {
                if (y >= 0 && y < size.second && x >= 0 && x < size.first) {
                    if (mapMatrix[y][x] != 1) {
                        canPlace = false;
                    }
                }
            }
        }
        if (canPlace) 
        {
            bool isHollow = std::uniform_int_distribution<>(0, 1)(gen);
            for (int y = startY; y < startY + clusterHeight && y < size.second - 1; y++) {
                for (int x = startX; x < startX + clusterWidth && x < size.first - 1; x++) {
                    if (!isHollow || y == startY || y == startY + clusterHeight - 1 ||
                        x == startX || x == startX + clusterWidth - 1) {
                        mapMatrix[y][x] = 2;
                        bool isDestructible = (wallTypeDist(gen) == 1);
                        WallType type = isDestructible ? WallType::Destructible : WallType::NonDestructible;
                        walls.emplace_back(std::make_pair(y, x), type, 1, isDestructible, nullptr);
                    }
                }
            }
        }
    }

    std::uniform_int_distribution<> connectorDist(0, 3);
    for (int y = 1; y < size.second - 1; y++) {
        for (int x = 1; x < size.first - 1; x++) {
            if (mapMatrix[y][x] == 1) {
                bool hasNearbyWalls = false;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        if (mapMatrix[y + dy][x + dx] == 2) {
                            hasNearbyWalls = true;
                            break;
                        }
                    }
                }

                if (hasNearbyWalls && connectorDist(gen) == 0) {
                    mapMatrix[y][x] = 2;
                    bool isDestructible = (wallTypeDist(gen) == 1);
                    WallType type = isDestructible ? WallType::Destructible : WallType::NonDestructible;
                    walls.emplace_back(std::make_pair(y, x), type, 1, isDestructible, nullptr);
                }
            }
        }
    }

    std::vector<std::pair<int, int>> startPositions = {
        {0, 0}, {0, size.first - 1},
        {size.second - 1, 0}, {size.second - 1, size.first - 1}
    };
    std::shuffle(startPositions.begin(), startPositions.end(), gen);

    for (int i = 0; i < numPlayers; i++) {
        int py = startPositions[i].first;
        int px = startPositions[i].second;
        mapMatrix[py][px] = 0;
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int ny = py + dy;
                int nx = px + dx;
                if (ny >= 0 && ny < size.second && nx >= 0 && nx < size.first) {
                    if (mapMatrix[ny][nx] == 2) { 
                        mapMatrix[ny][nx] = 1; 
                        walls.erase(
                            std::remove_if(walls.begin(), walls.end(),
                                [ny, nx]( Wall& wall) {
                                    return wall.GetPosition() == std::make_pair(ny, nx);
                                }),
                            walls.end()
                        );
                    }
                }
            }
        }
    }

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
        }
    }

    std::cout << "Map generated with size (" << size.first << ", " << size.second << ") and " << numPlayers << " players." << std::endl;
    for (const auto& row : mapMatrix) {
        for (int cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}
bool Map::IsPositionFree(std::pair<int,int> position)
{
    for ( auto wall : walls) {
        if (wall.GetPosition() == position && wall.GetDurability() < 0) {
            return false;
        }
    }
    return true;
}

std::vector<Wall>& Map::GetWalls()
{
    return walls;
}

std::vector<Bomb>& Map::GetBombs()
{
    return bombs; 
}

std::pair<int, int> Map::GetSize()
{
    return size;
}

Wall* Map::GetWallAt(int x, int y)
{
    for (auto& wall : walls)
    {
        if (wall.GetPosition() == std::make_pair(x, y))
        {
            return& wall;
        }
    }
    return nullptr;
}

Bomb* Map::GetBombAt(int x, int y)
{
    for (auto& bomb : bombs)
    {
        if (bomb.GetPosition() == std::make_pair(x, y))
        {
            return& bomb;
        }
    }
    return nullptr;
}

void Map::SetFreePosition(int x, int y)
{
    mapMatrix[x][y] = 1;
}

