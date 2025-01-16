#include "Map.h"


Map::Map() : height(18), width(30) {
    mapMatrix.resize(height, std::vector<int>(width, 1));
}

Map::Map(size_t h, size_t w) : height(h), width(w) {
    mapMatrix.resize(height, std::vector<int>(width, 1));
}

//Map::Map(MapGenerator& generator) {
//    // Initialize mapMatrix from the generator
//    for (size_t i = 0; i < mapMatrix.size(); ++i) {
//        for (size_t j = 0; j < mapMatrix[i].size(); ++j) {
//            mapMatrix[i][j] = generator.GetMapMatrix()[i][j];
//        }
//    }
//
//    // Use walls from the generator
//    const auto& wallPositions = generator.GetWallPositions();
//    const auto& wallDurabilities = generator.GetWallDurabilities();
//    const auto& wallDestructibles = generator.GetWallDestructibleFlags();
//
//    // Assuming wallPositions, wallDurabilities, and wallDestructibles are all of the same size
//    for (size_t i = 0; i < wallPositions.size(); ++i) {
//        walls.push_back(std::make_unique<Wall>(
//            wallPositions[i],  // Position from GetWallPositions
//            wallDestructibles[i] ? WallType::DestructibleWall : WallType::NonDestructibleWall,  // Fixed enum usage
//            wallDurabilities[i],  // Durability from GetWallDurabilities
//            wallDestructibles[i]  // Destructibility status from GetWallDestructibleFlags
//        ));
//    }
//
//    // Use bombs from the generator
//    const auto& bombPositions = generator.GetBombPositions();
//    const auto& bombStatuses = generator.GetBombStatuses();
//
//    size_t bombIndex = 0;  // Index to insert bombs into the array
//    for (size_t i = 0; i < bombPositions.size(); ++i) {
//        if (bombStatuses[i] && bombIndex < bombs.size()) {
//            bombs[bombIndex++] = std::make_unique<Bomb>(bombPositions[i]);
//        }
//    }
//}
Map::Map(MapGenerator& generator) {
    // Get dimensions from generator
    height = generator.GetHeightG();
    width = generator.GetWidthG();

    // Initialize mapMatrix with the correct size
    mapMatrix.resize(height, std::vector<int>(width));

    // Copy the map data
    const auto& generatorMatrix = generator.GetMapMatrix();
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            mapMatrix[i][j] = generatorMatrix[i][j];
        }
    }

    // Use walls from the generator
    const auto& wallPositions = generator.GetWallPositions();
    const auto& wallDurabilities = generator.GetWallDurabilities();
    const auto& wallDestructibles = generator.GetWallDestructibleFlags();

    for (size_t i = 0; i < wallPositions.size(); ++i) {
        walls.push_back(std::make_unique<Wall>(
            wallPositions[i],
            wallDestructibles[i] ? WallType::DestructibleWall : WallType::NonDestructibleWall,
            wallDurabilities[i],
            wallDestructibles[i]
        ));
    }

    // Use bombs from the generator
    const auto& bombPositions = generator.GetBombPositions();
    const auto& bombStatuses = generator.GetBombStatuses();

    size_t bombIndex = 0;
    for (size_t i = 0; i < bombPositions.size(); ++i) {
        if (bombStatuses[i] && bombIndex < bombs.size()) {
            bombs[bombIndex++] = std::make_unique<Bomb>(bombPositions[i]);
        }
    }
}
 
std::vector<std::pair<int, int>> Map::GetPlayerStartPositions() const {
    std::vector<std::pair<int, int>> startPositions;
    for (size_t i = 0; i < mapMatrix.size(); ++i) {
        for (size_t j = 0; j < mapMatrix[i].size(); ++j) {
            if (mapMatrix[i][j] == 0) {  // 0 indică poziția unui jucător
                startPositions.emplace_back(i, j);
            }
        }
    }
    return startPositions;
}


bool Map::IsPositionFree(std::pair<int, int> position) const 
{
    for (const auto& wall : walls) {  // Use reference to avoid copying
        if (wall->GetPosition() == position && wall->GetDurability() < 0) {
            return false;
        }
    }
    return true;
}

const std::vector<std::vector<int>>& Map::GetMapMatrix() const {
    return mapMatrix;
}


bool Map::IsMovable(int x, int y) const 
{
    if (x < 0 || x >= height || y < 0 || y >= width) { 
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

size_t Map::GetHeight() const 
{
    return height;
}

size_t Map::GetWidth() const
{
    return width;
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


int Map::GetCellValue(int x, int y) const {
    if (x < 0 || x >= height || y < 0 || y >= width) {
        return -1;  // Celulă invalidă
    }
    return mapMatrix[x][y];
}

void Map::SetCellValue(int x, int y, int value) {
    if (x >= 0 && x < height && y >= 0 && y < width) {
        mapMatrix[x][y] = value;
    }
}
