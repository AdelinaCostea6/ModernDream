#pragma once
#include "Map.h"
#include "Player.h"
#include "Wall.h"
import bomb;

class Game
{
private:
    Map map;
    std::array<std::shared_ptr<Player>, 4> players; 
    std::vector<std::shared_ptr<Wall>> walls;       
    std::array<std::shared_ptr<Bomb>, 3> bombs;     

public:
    Game(std::vector<std::shared_ptr<Wall>> walls,
        std::vector<std::shared_ptr<Bomb>> bombs,
        std::vector<std::shared_ptr<Player>> players);

    const Map& GetMap() const;
    std::vector<std::shared_ptr<Player>> GetPlayers() const;
    const std::vector<std::shared_ptr<Wall>>& GetWalls() const;
    std::vector<std::shared_ptr<Bomb>> GetBombs() const;
    void DetermineWinner();
    void WinGame();
    void FinishSecond();
    void CheckAndApplyWeaponUpgrade();
    void TriggerBomb(int x, int y);
};
