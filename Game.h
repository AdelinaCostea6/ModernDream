#pragma once
#include "Map.h"
#include "Player.h"
#include "Bomb.h"

class Game
{
private:
    Map map;
    std::vector<Player> players;
    std::vector<Bomb> bombs;
public:
    void DetermineWinner();
    void WinGame(Player* player);
    void FinishSecond(Player* player);
    void CheckAndApplyWeaponUpgrade();
    std::vector<Player>& GetPlayers() { return players; } 
};

