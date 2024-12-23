#include "Map.h"


Map::Map() {
    for (auto& row : mapMatrix) {
        row.fill(1);  // Initialize map matrix with free space
    }
}
Map::Map(MapGenerator& generator) {
    // Initialize mapMatrix from the generator
    for (size_t i = 0; i < mapMatrix.size(); ++i) {
        for (size_t j = 0; j < mapMatrix[i].size(); ++j) {
            mapMatrix[i][j] = generator.GetMapMatrix()[i][j];
        }
    }

    // Use walls from the generator
    const auto& wallPositions = generator.GetWallPositions();
    const auto& wallDurabilities = generator.GetWallDurabilities();
    const auto& wallDestructibles = generator.GetWallDestructibleFlags();

    // Assuming wallPositions, wallDurabilities, and wallDestructibles are all of the same size
    for (size_t i = 0; i < wallPositions.size(); ++i) {
        walls.push_back(std::make_unique<Wall>(
            wallPositions[i],  // Position from GetWallPositions
            wallDestructibles[i] ? WallType::Destructible : WallType::NonDestructible,  // Wall type based on destructibility
            wallDurabilities[i],  // Durability from GetWallDurabilities
            wallDestructibles[i]  // Destructibility status from GetWallDestructibleFlags
        ));
    }

    // Use bombs from the generator
    const auto& bombPositions = generator.GetBombPositions();
    const auto& bombStatuses = generator.GetBombStatuses();

    size_t bombIndex = 0;  // Index to insert bombs into the array
    for (size_t i = 0; i < bombPositions.size(); ++i) {
        if (bombStatuses[i] && bombIndex < bombs.size()) {
            bombs[bombIndex++] = std::make_unique<Bomb>(bombPositions[i]);  // Manually insert bombs
        }
    }
}
 


bool Map::IsPositionFree(std::pair<int, int> position)
{
    for (const auto& wall : walls) {  // Use reference to avoid copying
        if (wall->GetPosition() == position && wall->GetDurability() < 0) {
            return false;
        }
    }
    return true;
}


bool Map::IsMovable(int x, int y)
{
    if (x < 0 || x >= kHeight || y < 0 || y >= kWidth) { 
        return false;
    }
    if (!IsPositionFree({ x, y })) {
        return false;
    }

    return mapMatrix[x][y] == 1;
}

const std::vector<std::unique_ptr<Wall>>& Map::GetWalls() 
{
    return walls;
}

const std::array<std::unique_ptr<Bomb>, 3>& Map::GetBombs()
{
    return bombs; 
}

size_t Map::GetHeight()
{
    return kHeight;
}

size_t Map::GetWidth()
{
    return kWidth;
}

//std::pair<int, int> Map::GetSize()
//{
//    return size;
//}

Wall* Map::GetWallAt(int x, int y) {
    for (auto& wall : walls) {
        if (wall->GetPosition() == std::make_pair(x, y)) {
            return wall.get();  // Use .get() to return the raw pointer
        }
    }
    return nullptr;
}

std::unique_ptr<Bomb>* Map::GetBombAt(int x, int y)
{
    for (auto& bomb : bombs)
    {
        if (bomb->GetPosition() == std::make_pair(x, y))
        {
            return &bomb;  
        }
    }
    return nullptr;
}

void Map::SetFreePosition(int x, int y)
{
    mapMatrix[x][y] = 1;
}

void Map::SetWalls(std::vector<std::unique_ptr<Wall>> newWalls) 
{
    walls = std::move(newWalls);
}

//void Map::SetBombs(const std::array<std::unique_ptr<Bomb>, 3>&& newBombs)
//{
//    bombs = std::move(newBombs);  
//}
void Map::SetBombs(const std::array<std::unique_ptr<Bomb>, 3>& newBombs)
{
    for (size_t i = 0; i < bombs.size(); ++i) {
        bombs[i] = newBombs[i] ? std::make_unique<Bomb>(*newBombs[i]) : nullptr;
    }
}

