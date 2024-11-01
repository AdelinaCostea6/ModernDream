#include "Map.h"

Map::Map(int dimLinii, int dimColoane, std::vector<std::vector<int>> matrix)
{
    this->dimLinii = dimLinii;
    this->dimColoane = dimColoane;
    this->matrix = matrix;
};
