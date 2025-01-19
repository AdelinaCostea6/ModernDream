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

    DatabaseManager dbManager;
    http::Routing routing;
    routing.Run(dbManager);
    return 0;
}


//#include <iostream>
//#include "GameSessionManager.h"
//#include "Player.h"
//#include "Game.h"
//#include "Map.h"
//
//void TestInitialization() {
//    // 1. Creează o instanță de GameSessionManager
//    GameSessionManager sessionManager;
//
//    // 2. Creează o listă de WaitingPlayer pentru a simula jucătorii în așteptare
//    std::array<std::unique_ptr<WaitingPlayer>, 4> waitingPlayers = {
//        std::make_unique<WaitingPlayer>("Player1", 100),
//        std::make_unique<WaitingPlayer>("Player2", 100),
//        std::make_unique<WaitingPlayer>("Player3", 100),
//        std::make_unique<WaitingPlayer>("Player4", 100)
//    };
//
//    // 3. Creează o sesiune și inițializează jucătorii
//    sessionManager.CreateMatch(std::move(waitingPlayers));
//
//    // 4. Obține sesiunea creată
//    std::string sessionId = sessionManager.CreateSession(4);  // Numărul de jucători necesari
//    auto session = sessionManager.GetSession(sessionId);
//
//    if (session) {
//        std::cout << "[TEST] Session created with ID: " << sessionId << "\n";
//
//        // 5. Verifică jucătorii din sesiune
//        std::cout << "[TEST] Players in session:\n";
//        for (const auto& [username, player] : session->players) {
//            if (player) {
//                std::cout << "- " << username << " at position ("
//                    << player->GetPosition().first << ", "
//                    << player->GetPosition().second << ")\n";
//            }
//            else {
//                std::cout << "- Player " << username << " is null\n";
//            }
//        }
//
//        // 6. Verifică harta și pozițiile jucătorilor
//       /* std::cout << "[TEST] Map and player positions:\n";
//        session->game.UpdateMapWithPlayers();
//        for (int row = 0; row < session->game.GetMap().GetHeight(); ++row) {
//            for (int col = 0; col < session->game.GetMap().GetWidth(); ++col) {
//                std::cout << session->game.GetMap().GetCellValue(row, col) << " ";
//            }
//            std::cout << "\n";
//        }*/
//    }
//    else {
//        std::cerr << "[TEST ERROR] Failed to create session!\n";
//    }
//}
//
//int main() {
//    GameSessionManager sessionManager;
//
//    // Simulează jucătorii în așteptare
//    std::array<std::unique_ptr<WaitingPlayer>, 4> players = {
//        std::make_unique<WaitingPlayer>("Player1", 100),
//        std::make_unique<WaitingPlayer>("Player2", 200),
//        std::make_unique<WaitingPlayer>("Player3", 300),
//        std::make_unique<WaitingPlayer>("Player4", 400)
//    };
//
//    // Creează sesiunea o singură dată
//    sessionManager.CreateMatch(std::move(players));
//    //sessionManager.CreateSession(4);
//
//    // Test: obține sesiunea creată
//    std::string sessionId = sessionManager.CreateSession(4);  // Numărul de jucători necesari
//    auto session = sessionManager.GetSession(sessionId);
//    //auto session = sessionManager.GetSession("32819");  // Folosește ID-ul real
//
//    if (session) {
//        std::cout << "[TEST] Players in session:\n";
//        for (const auto& [username, player] : session->players) {
//            if (player) {
//                std::cout << "- " << username << " at position ("
//                    << player->GetPosition().first << ", "
//                    << player->GetPosition().second << ")\n";
//            }
//        }
//    }
//
//    return 0;
//}
//
