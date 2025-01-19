
#pragma once
#include "Map.h"
#include "Player.h"
#include <array>
#include <deque>
#include "Bullet.h"
#include <map>

class Game {
private:
    Map map;
    std::array<std::unique_ptr<Player>, 4> players;
    std::deque<Bullet> bullets;
    std::vector<std::pair<int, int>> updatedCells;
    std::map<std::string, std::pair<int, int>> playerPositions;
    std::map<int, std::string> bulletToPlayerMap;
    int bulletCounter = 0;

public:
    Game() = default;
    Game(Map map, const std::map<std::string, std::unique_ptr<Player>>& sessionPlayers);

    const Map& GetMap() const;
    std::array<std::unique_ptr<Player>, 4>& GetPlayers();
    void DetermineWinner();
    void WinGame();
    void FinishSecond();
    void CheckAndApplyWeaponUpgrade();
    void TriggerBomb(int x, int y);
    void GenerateMap(int numPlayers);
    void ShootBullet(const Player& player);
    void UpdateBullets();
    const std::deque<Bullet>& GetBullets() const;
    const std::vector<std::pair<int, int>>& GetUpdatedCells() const;
    void ClearUpdatedCells();
    std::map<std::string, std::pair<int, int>> GetPlayerPositions() const;
    void UpdatePlayerPosition(const std::string& username, int x, int y);
    std::unique_ptr<Player>GetPlayerByName(const std::string& name);
};

