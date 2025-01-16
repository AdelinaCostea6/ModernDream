#pragma once

//struct BulletInfo 
//{
//    int x;  // Coordonata X
//    int y;  // Coordonata Y
//   
//
//    BulletInfo(int x, int y) : x(x), y(y){} 
//};

struct BulletInfo {
    int x;       // Poziția X
    int y;       // Poziția Y
    char direction;  // Direcția glonțului: 'w', 'a', 's', sau 'd'

    BulletInfo(int x, int y, char direction) : x(x), y(y), direction(direction) {}
};

