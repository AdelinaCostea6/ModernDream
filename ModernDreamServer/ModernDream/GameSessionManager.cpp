#include "GameSessionManager.h"
#include <stdexcept>
#include <random>
#include <iostream>
#include "Game.h"

// Constructor de mutare
//GameSession::GameSession(GameSession&& other) noexcept
//    : sessionId(std::move(other.sessionId)),
//    requiredPlayers(other.requiredPlayers),
//    players(std::move(other.players)),
//    isReady(other.isReady),
//    game(std::move(other.game))  // Mutăm membrul `game`
//{
//}
//
//
//// Operator de atribuire prin mutare
//GameSession& GameSession::operator=(GameSession&& other) noexcept {
//    if (this != &other) {
//        sessionId = std::move(other.sessionId);
//        requiredPlayers = other.requiredPlayers;
//        players = std::move(other.players);
//        isReady = other.isReady;
//        game = std::move(other.game);  // Mutăm membrul `game`
//    }
//    return *this;
//}
//std::string GameSessionManager::CreateSession(int requiredPlayers) {
//
//    // Utilizare random_device pentru seed unic
//    std::random_device rd;
//    std::mt19937 gen(rd()); // Motorul Mersenne Twister
//    std::uniform_int_distribution<> dis(1, 1000000);
//
//    std::string sessionId = std::to_string(dis(gen));
//   // std::string sessionId = std::to_string(rand());
//    sessions.emplace(sessionId, GameSession(sessionId, requiredPlayers)); 
//    /*std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_int_distribution<> dis(0, std::numeric_limits<int>::max());
//    std::string sessionId = std::to_string(dis(gen));
//    sessions.emplace(sessionId, GameSession(sessionId, requiredPlayers)); */
//    return sessionId;
//}
//
//
////NU
////bool GameSessionManager::JoinSession(const std::string& sessionId, const std::string& username) {
////    auto it = sessions.find(sessionId);
////    if (it != sessions.end() && !it->second.isReady) {
////        auto& session = it->second;
////
////        if (session.players.find(username) == session.players.end()) {
////            auto weapon = std::make_unique<Weapon>();
////            auto player = std::make_unique<Player>(username, std::move(weapon), std::make_pair(0, 0));
////
////            session.players[username] = std::move(player);
////            //session.players.emplace(username, std::move(player)); 
////
////            if (session.players.size() >= session.requiredPlayers) {
////                session.isReady = true;
////            }
////            return true;
////        }
////    }
////    return false;
////}
//bool GameSessionManager::JoinSession(const std::string& sessionId, const std::string& username) {
//    auto it = sessions.find(sessionId);
//    if (it != sessions.end() && !it->second->isReady) {
//        auto& session = it->second;
//
//        if (session->players.find(username) == session->players.end()) {
//            auto weapon = std::make_unique<Weapon>();
//            auto player = std::make_unique<Player>(username, std::move(weapon), std::make_pair(0, 0));
//
//            session->players[username] = std::move(player);
//            session->lastJoinedPlayer = username;  // Track last joined player
//            session->lastLeftPlayer = "";  // Reset last left
//
//            std::cout << "Player " << username << " joined session " << sessionId
//                << ". Total players: " << session->players.size() << std::endl;
//
//            if (session->players.size() >= session->requiredPlayers) {
//                session->isReady = true;
//            }
//            return true;
//        }
//    }
//    return false;
//}
//
//void GameSessionManager::LeaveSession(const std::string& sessionId, const std::string& username) {
//    auto it = sessions.find(sessionId);
//    if (it != sessions.end()) {
//        auto& session = it->second;
//        session->players.erase(username);
//        session->lastLeftPlayer = username;  // Track last left player
//        session->lastJoinedPlayer = "";  // Reset last joined
//
//        if (session->players.size() < session->requiredPlayers) {
//            session->isReady = false;
//        }
//    }
//}
//
////NU
////bool GameSessionManager::JoinSession(const std::string& sessionId, const std::string& username) {
////    auto it = sessions.find(sessionId);
////    if (it != sessions.end() && !it->second.isReady) {
////        auto& session = it->second;
////
////        if (session.players.find(username) == session.players.end()) {
////            auto weapon = std::make_unique<Weapon>();
////            auto player = std::make_unique<Player>(username, std::move(weapon), std::make_pair(0, 0));
////
////            session.players[username] = std::move(player);
////
////            if (session.players.size() >= session.requiredPlayers) {
////                session.isReady = true;
////            }
////            return true;
////        }
////    }
////    return false;
////}
////
////
////void GameSessionManager::LeaveSession(const std::string& sessionId, const std::string& username) {
////    auto it = sessions.find(sessionId);
////    if (it != sessions.end()) {
////        auto& session = it->second;
////        session.players.erase(username);
////        if (session.players.size() < session.requiredPlayers) {
////            session.isReady = false;
////        }
////    }
////}
////NU
// /*GameSession GameSessionManager::GetSessionStatus(const std::string& sessionId) {
//    auto it = sessions.find(sessionId);
//    if (it != sessions.end()) {
//        return std::move(it->second);
//    }
//    throw std::out_of_range("Session not found");
//}*/
//
//const GameSession& GameSessionManager::GetSessionStatus(const std::string& sessionId) const {
//    auto it = sessions.find(sessionId);
//    if (it != sessions.end()) {
//        return *(it->second); // Returnează sesiunea prin referință
//    }
//    throw std::out_of_range("Session not found");
//}
//
//std::shared_ptr<GameSession> GameSessionManager::GetSession(const std::string& sessionId) {
//    auto it = sessions.find(sessionId);
//    if (it != sessions.end()) {
//        return it->second;
//    }
//    return nullptr;
//}

// Creează o sesiune nouă și returnează un ID unic
std::string GameSessionManager::CreateSession(int requiredPlayers) {
    //requiredPlayers = 4;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);
    std::string sessionId = std::to_string(dis(gen));  // Generează un ID unic pentru sesiune

    // Creăm sesiunea și o adăugăm în mapă
    sessions[sessionId] = std::make_shared<GameSession>(sessionId, requiredPlayers);
    std::shared_ptr<GameSession> session = std::make_shared<GameSession>(sessionId, requiredPlayers);
    //session->game.SetMapType(mapType);  // Set the map type
    sessions[sessionId] = session;

    std::cout << "Sesiune creată cu ID: " << sessionId << " și " << requiredPlayers << " jucători necesari.\n";
    return sessionId;
}

// Jucătorul se alătură unei sesiuni
bool GameSessionManager::JoinSession(const std::string& sessionId, const std::string& username) {
    auto it = sessions.find(sessionId);
    if (it != sessions.end() && !it->second->isReady) {
        auto session = it->second;

        // Dacă jucătorul nu este deja în sesiune, îl adăugăm
        if (session->players.find(username) == session->players.end()) {
            auto weapon = std::make_unique<Weapon>();
            auto player = std::make_unique<Player>(username, std::move(weapon), std::make_pair(0, 0));
            session->players[username] = std::move(player);

            session->lastJoinedPlayer = username;
            session->lastLeftPlayer = "";

            std::cout << "Jucătorul " << username << " s-a alăturat sesiunii " << sessionId << "\n";

            // Setăm sesiunea ca "gata" dacă s-au atins numărul de jucători necesari
            if (session->players.size() >= session->requiredPlayers) {
               // session->game.GenerateMap(session->requiredPlayers);
                session->isReady = true;
            }
            return true;
        }
    }
    return false;
}

// Jucătorul părăsește sesiunea
void GameSessionManager::LeaveSession(const std::string& sessionId, const std::string& username) {
    auto it = sessions.find(sessionId);
    if (it != sessions.end()) {
        auto session = it->second;

        session->players.erase(username);
        session->lastLeftPlayer = username;
        session->lastJoinedPlayer = "";

        std::cout << "Jucătorul " << username << " a părăsit sesiunea " << sessionId << "\n";

        // Dacă numărul de jucători este sub cel necesar, sesiunea nu mai este "gata"
        if (session->players.size() < session->requiredPlayers) {
            session->isReady = false;
        }
    }
}

// Obține sesiunea cu un anumit ID
std::shared_ptr<GameSession> GameSessionManager::GetSession(const std::string& sessionId) {
    auto it = sessions.find(sessionId);
    if (it != sessions.end()) {
        return it->second;
    }
    return nullptr;
}

// Returnează starea unei sesiuni
const GameSession& GameSessionManager::GetSessionStatus(const std::string& sessionId) const {
    auto it = sessions.find(sessionId);
    if (it != sessions.end()) {
        return *(it->second);
    }
    throw std::out_of_range("Sesiunea nu a fost găsită.");
}

std::map<std::string, std::shared_ptr<GameSession>>& GameSessionManager::GetSessions()
{
    return sessions;
}

void GameSessionManager::AddToQueue(const std::string& username, int score) {
    auto player = std::make_unique<WaitingPlayer>(username, score); // Create a unique_ptr
    waitingQueue.push_back(std::move(player)); // Move the unique_ptr into the queue
    std::cout << "Player " << username << " added to queue with score: " << score << std::endl;
}


void GameSessionManager::MatchPlayers() {
    auto now = std::chrono::steady_clock::now();

    while (waitingQueue.size() >= 2) {
        std::array<std::unique_ptr<WaitingPlayer>, 4> selectedPlayers = { nullptr, nullptr, nullptr, nullptr };
        size_t index = 0;

        // Select players based on timeout or required number
        for (auto it = waitingQueue.begin(); it != waitingQueue.end() && index < 4; ) {
            if (std::chrono::duration_cast<std::chrono::seconds>(now - (*it)->joinTime).count() >= 30 || index < 4) {
                selectedPlayers[index++] = std::move(*it); // Transfer ownership
                it = waitingQueue.erase(it);
            }
            else {
                ++it;
            }

        }

        if (index >= 2) { // At least 2 players are required to start a session
            CreateMatch(std::move(selectedPlayers));
        }
    }
}



void GameSessionManager::CreateMatch(std::array<std::unique_ptr<WaitingPlayer>, 4> players) {
    // Create a new session
    std::string sessionId = CreateSession(players.size());
    auto session = GetSession(sessionId);

    // Add players to the session
    for (const auto& player : players) {
        if (player) {
            JoinSession(sessionId, player->username);
        }
    }

    session->isReady = true;

    // Output session details
    std::cout << "Game session " << sessionId << " created with players: ";
    for (const auto& player : players) {
        if (player) {
            std::cout << player->username << " ";
        }
    }
    std::cout << std::endl;

    // No need to notify players since the game starts automatically
}



void GameSessionManager::ManageSession(const std::string& sessionId) {
    // Retrieve the session
    std::shared_ptr<GameSession> session;
    {
        std::lock_guard<std::mutex> lock(sessionMutex);
        auto it = sessions.find(sessionId);
        if (it == sessions.end()) {
            std::cerr << "Session " << sessionId << " not found." << std::endl;
            return;
        }
        session = it->second;
    }

    // Start the game loop
    std::cout << "Game session " << sessionId << " started." << std::endl;
    auto& game = session->GetGame();

    //while (!game.IsGameOver()) {
    //    // Update bullets
    //    game.UpdateBullets();

    //    // Handle collisions and bomb triggers
    //    game.GetMap().CheckCollisions(game.GetPlayers());
    //    for (const auto& cell : game.GetUpdatedCells()) {
    //        std::cout << "Updated cell: (" << cell.first << ", " << cell.second << ")" << std::endl;
    //    }
    //    game.ClearUpdatedCells();

    //    // Delay to simulate game tick
    //    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //}

    // Determine the winner
    game.DetermineWinner();

    // Cleanup
    {
        std::lock_guard<std::mutex> lock(sessionMutex);
        sessions.erase(sessionId);
    }

    std::cout << "Game session " << sessionId << " ended." << std::endl;
}

