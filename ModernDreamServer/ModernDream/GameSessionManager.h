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

    GameSession() : sessionId(""), requiredPlayers(0), isReady(false) {}

    GameSession(const std::string& id, int reqPlayers)
        : sessionId(id), requiredPlayers(reqPlayers), isReady(false) {}

    GameSession(GameSession&& other) noexcept;
    GameSession& operator=(GameSession&& other) noexcept;

    GameSession(const GameSession&) = delete;
    GameSession& operator=(const GameSession&) = delete;
};

class GameSessionManager {
private:
    std::map<std::string, GameSession> sessions;

public:
    GameSessionManager() = default;

    std::string CreateSession(int requiredPlayers);
    bool JoinSession(const std::string& sessionId, const std::string& username);
    void LeaveSession(const std::string& sessionId, const std::string& username);
    GameSession GetSessionStatus(const std::string& sessionId);
};

