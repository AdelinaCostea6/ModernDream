#pragma once
#include "Map.h"
#include "Player.h"

class Game
{
private:
    Map map;
    std::vector<Player> players;
   

public:
    Game(Map map, std::vector<Player> players);
    Map GetMap();
    std::vector<Player> GetPlayers();
    void DetermineWinner();
    void WinGame();
    void FinishSecond();
    void CheckAndApplyWeaponUpgrade();
    void TriggerBomb(int x, int y);

};
