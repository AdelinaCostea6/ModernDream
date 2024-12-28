#include "GameSessionManager.h"
#include <stdexcept>
#include <iostream>
#include <random>
#include <memory>

std::string GameSessionManager::CreateSession(int requiredPlayers) {
    std::string sessionId = std::to_string(rand());
    sessions[sessionId] = GameSession{
        sessionId,
        requiredPlayers,
        std::map<std::string, std::unique_ptr<Player>>{},
        false
    };
    return sessionId;
}

bool GameSessionManager::JoinSession(const std::string& sessionId, const std::string& username) {
    auto it = sessions.find(sessionId);
    if (it != sessions.end() && !it->second.isReady) {
        auto& session = it->second;

        if (session.players.find(username) == session.players.end()) {
            
            auto weapon = std::make_unique<Weapon>();
            auto player = std::make_unique<Player>(username, std::move(weapon), std::make_pair(0, 0));

            session.players[username] = std::move(player);

            // Marchează sesiunea ca fiind pregătită dacă are suficienți jucători
            if (session.players.size() >= session.requiredPlayers) {
                session.isReady = true;
            }
            return true;
        }
    }
    return false;
}

void GameSessionManager::LeaveSession(const std::string& sessionId, const std::string& username) {
    auto it = sessions.find(sessionId);
    if (it != sessions.end()) {
        auto& session = it->second;
        session.players.erase(username);
        if (session.players.size() < session.requiredPlayers) {
            session.isReady = false;
        }
    }
}

GameSession GameSessionManager::GetSessionStatus(const std::string& sessionId) const {
    auto it = sessions.find(sessionId);
    if (it != sessions.end()) {
        return it->second;
    }
    throw std::out_of_range("Session not found");
}
