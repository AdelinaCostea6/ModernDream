#include "GameSessionManager.h"
#include <stdexcept>
#include <random>
#include <iostream>

// Constructor de mutare
GameSession::GameSession(GameSession&& other) noexcept
    : sessionId(std::move(other.sessionId)),
    requiredPlayers(other.requiredPlayers),
    players(std::move(other.players)),
    isReady(other.isReady) {}

// Operator de atribuire prin mutare
GameSession& GameSession::operator=(GameSession&& other) noexcept {
    if (this != &other) {
        sessionId = std::move(other.sessionId);
        requiredPlayers = other.requiredPlayers;
        players = std::move(other.players);
        isReady = other.isReady;
    }
    return *this;
}

std::string GameSessionManager::CreateSession(int requiredPlayers) {
    std::string sessionId = std::to_string(rand());
    sessions.emplace(sessionId, GameSession(sessionId, requiredPlayers)); 
    /*std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, std::numeric_limits<int>::max());
    std::string sessionId = std::to_string(dis(gen));
    sessions.emplace(sessionId, GameSession(sessionId, requiredPlayers)); */
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
            //session.players.emplace(username, std::move(player)); 

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
 GameSession GameSessionManager::GetSessionStatus(const std::string& sessionId) {
    auto it = sessions.find(sessionId);
    if (it != sessions.end()) {
        return std::move(it->second);
    }
    throw std::out_of_range("Session not found");
}
