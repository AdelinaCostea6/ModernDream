#include "LoginSystem.h"

auto LoginSystem::CreateStorage()
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

bool LoginSystem::ValidateUsername(const std::string& username)
{
	std::regex username_pattern("^[a-zA-Z][a-zA-Z0-9_]{2,19}$");
	return std::regex_match(username, username_pattern);
}

void LoginSystem::UpdatePlayerStats(const std::vector<Player>& players)
{
	auto storage = CreateStorage();
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

std::vector<Player> LoginSystem::RegisterPlayersForGame()
{
	auto storage = CreateStorage();
	storage.sync_schema();
	std::vector<Player>players;

	int playerCount;
	std::cout << "Enter number of players: \n";
	std::cin >> playerCount;
	std::cin.ignore();

	for (int i = 0; i < playerCount; ++i)
	{
		std::string username;
		std::cout << "Enter username for Player " << (i + 1) << ": ";
		std::getline(std::cin, username);

		if (!ValidateUsername(username))
		{
			std::cout << "Invalid username format\n";
			i--;
			continue;
		}

		auto userPtr = storage.get_pointer<UserData>(username);
		if (!userPtr)
		{
			UserData newUser{ username, 0, 0, 0 };
			storage.replace(newUser);
			userPtr = storage.get_pointer<UserData>(username);
			if (!userPtr)
			{
				std::cerr << "Failed to retrieve user data after insertion\n";
				return {};
			}
		}

		auto weapon = std::make_shared<Weapon>(1.0f);
		std::pair<int, int> startPos = { i * 5, i * 5 };
		Player player(username, weapon, startPos);

		player.SetScore(userPtr->totalScore);
		player.SetPoints(userPtr->totalPoints);
		players.push_back(player);
	}
	return players;
}
