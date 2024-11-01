#pragma once
#include <iostream>
#include <vector>

class Map
{
private:
    int dimLinii;
    int dimColoane;
    std::vector<std::vector<int>> matrix;

    Map(int linii, int coloane, std::vector<std::vector<int>> matrix);
};