#include "Map.h"


void Map::GenerateMap(int numPlayers)
{
    std::vector<std::vector<int>> mapMatrix(size.second, std::vector<int>(size.first, 1));

    std::vector<std::pair<int, int>> startPositions = {
        {0, 0}, {0, size.first - 1}, {size.second - 1, 0}, {size.second - 1, size.first - 1}
    };
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(startPositions.begin(), startPositions.end(), gen);
    for (int i = 0; i < numPlayers; i++) {
        mapMatrix[startPositions[i].first][startPositions[i].second] = 0;
    }

    std::uniform_int_distribution<> wallChance(0, 10);
    std::uniform_int_distribution<> wallType(0, 1);
    std::uniform_int_distribution<> wallDurability(1, 1);
    for (int i = 0; i < size.second; i++) {
        for (int j = 0; j < size.first; j++) {
            if (mapMatrix[i][j] == 1 && wallChance(gen) < 2) {
                mapMatrix[i][j] = 2;
                WallType type = (wallType(gen) == 0) ? WallType::NonDestructible : WallType::Destructible;
                int durability = wallDurability(gen);
                bool destructible = (type == WallType::Destructible);
                walls.emplace_back(std::make_pair(i, j), type, durability, destructible);
            }
        }
    }

    std::uniform_int_distribution<> bombCountDist(0, 3);
    int bombCount = bombCountDist(gen);

    std::uniform_int_distribution<> bombChance(0, 20);
    int bombsAdded = 0;

    for (int i = 0; i < size.second; i++) {
        for (int j = 0; j < size.first; j++) {
            if (mapMatrix[i][j] == 1 && bombChance(gen) < 1 && bombsAdded < bombCount) {
                bombs.emplace_back(std::make_pair(i, j));
                bombsAdded++;
            }
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
