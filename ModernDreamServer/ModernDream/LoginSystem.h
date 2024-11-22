#pragma once
#include <iostream>
#include <regex>
#include <sqlite_orm/sqlite_orm.h>
#include "Player.h"

struct UserData
{
	std::string username;
	int totalScore = 0;
	int totalPoints = 0;
	int gamesPlayed = 0;

	auto operator==(const UserData& other) const {
		return username == other.username;
	}
};
class LoginSystem
{
private:
	auto CreateStorage();
	bool ValidateUsername(const std::string& username);

public:
	void UpdatePlayerStats(const std::vector<Player>& players); 
	std::vector<Player> RegisterPlayersForGame();

};

