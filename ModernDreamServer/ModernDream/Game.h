#pragma once
#include "Map.h"
#include "Player.h"
#include "Wall.h"
#include "Bomb.h"

class Game
{
private:
    Map map;
    std::vector<Player> players;
    std::vector<Wall>walls;
    std::vector<Bomb>bombs;
   

public:
    Game( std::vector<Wall> walls,  std::vector<Bomb> bombs,  std::vector<Player> players);
    Map GetMap();
    std::vector<Player> GetPlayers();
    std::vector<Wall> GetWalls();
    std::vector<Bomb> GetBombs();
    void DetermineWinner();
    void WinGame();
    void FinishSecond();
    void CheckAndApplyWeaponUpgrade();
    void TriggerBomb(int x, int y);

};
