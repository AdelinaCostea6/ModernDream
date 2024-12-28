#include <iostream>
#include <conio.h>
#include <thread>
#include <vector> 
#include <memory>
#include <chrono>
#include "Bullet.h"
#include "Wall.h"
#include "Player.h"
#include "Map.h"
#include "Game.h"
#include "DatabaseManager.h"
#include "Routing.h"
import bomb;
#include "../MapGenerator/MapGenerator/MapGenerator.h"

int main() {
    /*std::cout << "=== Testing Classes ===\n\n";

    // Bomb Test
    std::cout << "--- BOMB TEST ---\n";
    Coordinates bombPos = { 5, 5 };
    Bomb bomb(bombPos);
    std::cout << "Bomb was created at position(" << bomb.GetPosition().first << "," << bomb.GetPosition().second << ")\n";
    std::cout << "Bomb status: " << (bomb.GetStatus() ? "active" : "inactive") << "\n\n";

    // Bullet Test
    std::cout << "--- BULLET TEST ---\n";
    Bullet bullet({ 1, 1 });
    std::cout << "A bullet was created at position: (" << bullet.GetPosition().first
        << ", " << bullet.GetPosition().second << ")\n";
    std::cout << "Bullet speed: " << bullet.GetSpeed() << "\n";
    bullet.Movement();
    bullet.SetDoubleSpeed();
    std::cout << "Speed after doubling: " << bullet.GetSpeed() << "\n\n";

    // Wall Test
    std::cout << "--- WALL TEST ---\n";
    Wall wall({ 3, 3 }, WallType::Destructible, 2, true);
    std::cout << "A wall was created at position: (" << wall.GetPosition().first
        << ", " << wall.GetPosition().second << ")\n";
    std::cout << "Durability: " << wall.GetDurability() << "\n";
    wall.ReduceDurability();
    std::cout << "Durability after reduce: " << wall.GetDurability() << "\n\n";

    // Weapon Test
    std::cout << "--- WEAPON TEST ---\n";
    Weapon weapon(1.0f);
    std::cout << "A weapon was created with the speed: " << weapon.GetSpeed() << "\n";
    std::cout << "Initial waiting time: " << weapon.GetWaitingTime() << "\n";
    weapon.UpgradeSpeed(0.5f);
    weapon.UpgradeWaitingTime(1.0f);
    std::cout << "After upgrade - Speed: " << weapon.GetSpeed()
        << ", Waiting time: " << weapon.GetWaitingTime() << "\n\n";

    // Map Generator Test
    std::cout << "--- MAP GENERATOR TEST ---\n";
    MapGenerator generator;
    std::cout << "Generating map for 2 players...\n";
    generator.GenerateMap(2);

    Map gameMap(generator);


    // Map Dimensions
    std::cout << "Map dimensions: " << gameMap.GetHeight() << "x" << gameMap.GetWidth() << "\n";

    // Walls Info
    const auto& walls = gameMap.GetWalls();
    std::cout << "Total number of walls: " << walls.size() << "\n";

    int destructibleCount = 0;
    int nonDestructibleCount = 0;
    for (const auto& wall : walls) {
        if (wall->IsDestructible()) destructibleCount++;
        else nonDestructibleCount++;
    }
    std::cout << "Destructible walls: " << destructibleCount << "\n";
    std::cout << "Non-destructible walls: " << nonDestructibleCount << "\n\n";

    // Bombs Info
    const auto& bombs = gameMap.GetBombs();
    int activeBombs = 0;
    for (const auto& bomb : bombs) {
        if (bomb && bomb->GetStatus()) activeBombs++;
    }
    std::cout << "Active bombs: " << activeBombs << "\n";

    // Player Test
    std::cout << "--- PLAYER TEST ---\n";
    auto playerWeapon = std::make_unique<Weapon>(1.0f);
    auto player1 = std::make_unique<Player>("Player1", std::move(playerWeapon), std::make_pair(0, 0));

    std::cout << "Player was created: " << player1->GetName() << "\n";
    std::cout << "Initial position: (" << player1->GetPosition().first
        << ", " << player1->GetPosition().second << ")\n\n";
    player1->Shoot();
    std::cout << "Remaining lives: " << player1->GetLifes() << "\n\n";

    while (true) {
        player1->Movement(gameMap);
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'q') {
                std::cout << "Exiting movement test.\n";
                    break;
            }
        }
    }

    // Game Test
    std::cout << "--- GAME TEST ---\n";
    auto player2 = std::make_unique<Player>("Player2", std::make_unique<Weapon>(1.0f),
        std::make_pair(gameMap.GetHeight() - 1, gameMap.GetWidth() - 1));

    std::array<std::unique_ptr<Player>, 4> players = { std::move(player1), std::move(player2), nullptr, nullptr };

    std::vector<std::unique_ptr<Wall>> gameWalls;
    for (const auto& wall : gameMap.GetWalls()) {
        gameWalls.push_back(std::make_unique<Wall>(
            wall->GetPosition(), wall->GetType(), wall->GetDurability(), wall->IsDestructible()
        ));
    }

    std::array<std::unique_ptr<Bomb>, 3> gameBombs;
    size_t bombIndex = 0;
    for (const auto& bomb : gameMap.GetBombs()) {
        if (bomb) {
            gameBombs[bombIndex++] = std::make_unique<Bomb>(bomb->GetPosition());
        }
    }

    Game game(std::move(gameWalls), std::move(gameBombs), std::move(players));


    /*std::cout << "Testing winner determination:\n";
    game.DetermineWinner();

    std::cout << "Testing weapon upgrades:\n";
    players[0]->SetScore(10);
    game.CheckAndApplyWeaponUpgrade();

    std::cout << "Testing bomb activation:\n";
    for (const auto& wall : game.GetWalls()) {
        if (wall->IsDestructible()) {
            auto pos = wall->GetPosition();
            std::cout << "Attempting to trigger bomb at position (" << pos.first << ", " << pos.second << ")\n";
            game.TriggerBomb(pos.first, pos.second);
            break;
        }
    }*/

    DatabaseManager dbManager;
    http::Routing routing;
    routing.Run(dbManager);

    return 0;
}

