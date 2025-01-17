#pragma once
#include <string>
#include <map>
#include <memory>
#include <mutex>
#include "Game.h"
#include <chrono>

//struct GameSession {
//    std::string sessionId;
//    int requiredPlayers;
//    std::map<std::string, std::unique_ptr<Player>> players;
//    bool isReady = false;
//
//    std::string lastJoinedPlayer; 
//    std::string lastLeftPlayer; 
//
//    GameSession() : sessionId(""), requiredPlayers(0), isReady(false) {}
//
//    GameSession(const std::string& id, int reqPlayers)
//        : sessionId(id), requiredPlayers(reqPlayers), isReady(false) {
//        std::cout << "GameSession created with sessionId: " << sessionId
//            << ". Players map size: " << players.size() << std::endl;
//    }
//
//    GameSession(GameSession&& other) noexcept;
//    GameSession& operator=(GameSession&& other) noexcept;
//
//    GameSession(const GameSession&) = delete;
//    GameSession& operator=(const GameSession&) = delete;
//};

struct GameSession {
    std::string sessionId;
    int requiredPlayers;
    std::map<std::string, std::unique_ptr<Player>> players;
    bool isReady = false;

    std::string lastJoinedPlayer;
    std::string lastLeftPlayer;

    Game game;  // Instanța jocului

    /*std::vector<std::pair<int, int>> wallUpdates;
    std::mutex updateMutex;*/

    // Constructorul parametrizat
    GameSession(const std::string& id, int reqPlayers)
        : sessionId(id), requiredPlayers(reqPlayers), game() {
        game.GenerateMap(requiredPlayers);
    }

    // Constructorul de mutare
    GameSession(GameSession&& other) noexcept
        : sessionId(std::move(other.sessionId)),
        requiredPlayers(other.requiredPlayers),
        players(std::move(other.players)),
        isReady(other.isReady),
        game(std::move(other.game)) {}

    // Operatorul de atribuire prin mutare
    GameSession& operator=(GameSession&& other) noexcept {
        if (this != &other) {
            sessionId = std::move(other.sessionId);
            requiredPlayers = other.requiredPlayers;
            players = std::move(other.players);
            isReady = other.isReady;
            game = std::move(other.game);
        }
        return *this;
    }

    // Returnează referința la harta jocului
    const Map& GetMap() const {
        return game.GetMap();  // Obține harta din obiectul Game
    }
    Game& GetGame() { return game; }

    Player* GetPlayerByUsername(const std::string& username) {
        auto it = players.find(username);  // Caută username-ul în mapă
        if (it != players.end()) {
            std::cout << "[DEBUG] Player found: " << username << std::endl;
            return it->second.get();  // Returnează pointerul brut către `Player`
        }
        else
        {
            std::cerr << "[DEBUG] Player not found: " << username << std::endl;
            return nullptr;
        }
       
    }


};

struct WaitingPlayer {
    std::string username;
    int score;
    std::chrono::time_point<std::chrono::steady_clock> joinTime;
    WaitingPlayer(const std::string& name, int scr)
        : username(name), score(scr), joinTime(std::chrono::steady_clock::now()) {
    }
};

class GameSessionManager {
private:
    std::map<std::string, std::shared_ptr<GameSession>> sessions;  // Mapa sesiunilor de joc
    //std::string mapType;
    std::deque<std::unique_ptr<WaitingPlayer>> waitingQueue;

    std::mutex sessionMutex;

public:
    GameSessionManager() = default;

    // Creează o nouă sesiune și returnează ID-ul sesiunii
    std::string CreateSession(int requiredPlayers);

    // Jucătorul se alătură unei sesiuni
    bool JoinSession(const std::string& sessionId, const std::string& username);

    // Jucătorul părăsește sesiunea
    void LeaveSession(const std::string& sessionId, const std::string& username);

    // Obține sesiunea după ID
    std::shared_ptr<GameSession> GetSession(const std::string& sessionId);

    // Obține starea unei sesiuni
    const GameSession& GetSessionStatus(const std::string& sessionId) const;

    std::map<std::string, std::shared_ptr<GameSession>>& GetSessions();
    void MatchPlayers();
    void CreateMatch(std::array<std::unique_ptr<WaitingPlayer>, 4> players);
    void AddToQueue(const std::string& username, int score);
    void ManageSession(const std::string& sessionId);
    std::string FindOrCreateSession(const std::string& username, int score);

    /*const std::string& GetMapType() const { return mapType; } 
    void SetMapType(const std::string& type) { mapType = type; }*/ 

    //std::vector<std::pair<int, int>> GetUpdatedCellsForSession(const std::string& sessionId);
    //void ClearUpdatedCellsForSession(const std::string& sessionId);

    
};
