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
};
class LoginSystem
{
private:
	auto createStorage();
	bool validateUsername(const std::string& username);

public:
	void updatePlayerStats(const std::vector<Player>& players); 

};

