#pragma once
#include <string>
#include <map>
#include <memory>
#include "Player.h"

struct GameSession {
    std::string sessionId;                        
    int requiredPlayers;                           
    std::map<std::string, std::unique_ptr<Player>> players; 
    bool isReady = false;                          
};


class GameSessionManager {
private:
    std::map<std::string, GameSession> sessions;

public:
    GameSessionManager() = default;

    std::string CreateSession(int requiredPlayers);

    bool JoinSession(const std::string& sessionId, const std::string& username);

    void LeaveSession(const std::string& sessionId, const std::string& username);

    GameSession GetSessionStatus(const std::string& sessionId) const;
};

