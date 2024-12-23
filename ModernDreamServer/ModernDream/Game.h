//#pragma once
//#include "Map.h"
//#include "Player.h"
//#include "Wall.h"
//import bomb;
//
//class Game
//{
//private:
//    Map map;
//    std::array<std::shared_ptr<Player>, 4> players; 
//    std::vector<std::shared_ptr<Wall>> walls;       
//    std::array<std::shared_ptr<Bomb>, 3> bombs;     
//
//public:
//    Game(std::vector<std::unique_ptr<Wall>> walls,
//        std::array<std::unique_ptr<Bomb>, 3> bombs,
//        std::vector<std::shared_ptr<Player>> players)
//        : walls(std::move(walls)), bombs(std::move(bombs)), players(std::move(players)) {
//    }
//
//    const Map& GetMap() const;
//    std::vector<std::shared_ptr<Player>> GetPlayers() const;
//    const std::vector<std::shared_ptr<Wall>>& GetWalls() const;
//    std::vector<std::shared_ptr<Bomb>> GetBombs() const;
//    void DetermineWinner();
//    void WinGame();
//    void FinishSecond();
//    void CheckAndApplyWeaponUpgrade();
//    void TriggerBomb(int x, int y);
//};
#pragma once
#include "Map.h"
#include "Player.h"
#include "Wall.h"
import bomb;
#include <vector>
#include <memory>
#include <array>
#include <algorithm>
#include <iostream>
#include <cmath>

class Game
{
private:
    Map map;
    std::array<std::unique_ptr<Player>, 4> players;
    std::vector<std::unique_ptr<Wall>> walls;
    std::array<std::unique_ptr<Bomb>, 3> bombs;

public:
    Game(std::vector<std::unique_ptr<Wall>> walls,
        std::array<std::unique_ptr<Bomb>, 3> bombs,
        std::array<std::unique_ptr<Player>, 4> players);

    const Map& GetMap() const;
    std::array<std::unique_ptr<Player>, 4>& GetPlayers();
    std::vector<std::unique_ptr<Wall>>& GetWalls();
    const std::array<std::unique_ptr<Bomb>, 3>& GetBombs() const;
    void DetermineWinner();
    void WinGame();
    void FinishSecond();
    void CheckAndApplyWeaponUpgrade();
    void TriggerBomb(int x, int y);
};
