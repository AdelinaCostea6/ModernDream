#include "MapGenerator.h"
#include <random>
#include <algorithm>
#include <iostream>

Map MapGenerator::GenerateMap(int width, int height, int numPlayers) {
    Map map(std::make_pair(width, height));
    std::vector<std::vector<int>> mapMatrix;
    std::vector<Wall> walls;
    std::vector<Bomb> bombs;

    InitializeMapMatrix(mapMatrix, width, height);
    GenerateClusters(mapMatrix, walls, width, height);
    PlaceConnectorWalls(mapMatrix, walls, width, height);
    SetPlayerStartPositions(mapMatrix, walls, width, height, numPlayers);
    PlaceBombs(mapMatrix, walls, bombs);

    return map;
}
