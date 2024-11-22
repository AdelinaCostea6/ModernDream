#include "LoginSystem.h"

auto LoginSystem::createStorage()
{
	using namespace sqlite_orm;
	return make_storage("terrain_titans_users.sqlite",
		make_table("users",
			make_column("username", &UserData::username, primary_key()),
			make_column("total_score", &UserData::totalScore),
			make_column("total_points", &UserData::totalPoints),
			make_column("games_played", &UserData::gamesPlayed)
		)

	);
}

bool LoginSystem::validateUsername(const std::string& username)
{
	std::regex username_pattern("^[a-zA-Z][a-zA-Z0-9_]{2,19}$");
	return std::regex_match(username, username_pattern);
}

void LoginSystem::updatePlayerStats(const std::vector<Player>& players)
{
	auto storage = createStorage();
	for (auto player : players)
	{
		auto usePtr = storage.get_pointer<UserData>(player.GetName());
		if (usePtr)
		{
			usePtr->totalScore = player.GetScore();
			usePtr->totalPoints = player.GetPoints();
			usePtr->gamesPlayed++;

			storage.update(*usePtr);
		}
	}
}
