#pragma once
#include <string>
#include <map>
#include <memory>
//#include "Player.h"
#include "Game.h"

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

    // Constructorul parametrizat
    GameSession(const std::string& id, int reqPlayers)
        : sessionId(id), requiredPlayers(reqPlayers), game() {}

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


class GameSessionManager {
private:
    std::map<std::string, std::shared_ptr<GameSession>> sessions;  // Mapa sesiunilor de joc

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
    
};
