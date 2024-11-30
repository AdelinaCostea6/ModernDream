#include "LoginSystem.h"
#include <filesystem>
auto LoginSystem::CreateStorage()
{
	if (!std::filesystem::exists("data")) {
		std::filesystem::create_directory("data");
	}
	using namespace sqlite_orm;
	auto storage=make_storage("terrain_titans_users.sqlite",
		make_table("users",
			make_column("username", &UserData::username, primary_key()),
			make_column("total_score", &UserData::totalScore),
			make_column("total_points", &UserData::totalPoints),
			make_column("games_played", &UserData::gamesPlayed)
		)

	);
	storage.sync_schema();
	return storage;
}

bool LoginSystem::ValidateUsername(const std::string& username)
{
	std::regex username_pattern("^[a-zA-Z][a-zA-Z0-9_]{2,19}$");
	return std::regex_match(username, username_pattern);
}

bool LoginSystem::IsUsernameUnique(const std::string& username) 
{
	auto storage = CreateStorage();
	auto user = storage.get_pointer<UserData>(username);
	return user == nullptr; 
}

bool LoginSystem::RegisterUser(const std::string& username) 
{
	if (!ValidateUsername(username)) 
	{
		std::cout << "The username is invalid!" << std::endl;
		return false;
	}

	if (!IsUsernameUnique(username)) 
	{
		std::cout << "The username is already taken!" << std::endl;
		return false;
	}

	auto storage = CreateStorage();
	UserData newUser{ username, 0, 0, 0 }; 
	storage.replace(newUser);
	std::cout << "The account was created succesfully!" << std::endl;
	return true;
}


bool LoginSystem::LoginUser(const std::string& username) 
{
	auto storage = CreateStorage();
	auto userPtr = storage.get_pointer<UserData>(username);
	if (userPtr) 
	{
		std::cout << "Login failed!" << std::endl;
		return true;
	}
	else 
	{
		std::cout << "User cannot be found!" << std::endl;
		return false;
	}
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
	std::vector<Player> players;

	int playerCount;
	std::cout << "Enter number of players (max 4): ";
	std::cin >> playerCount;
	std::cin.ignore();

	if (playerCount > 4) {
		std::cout << "Maximum 4 players allowed.\n";
		playerCount = 4;
	}

	for (int i = 0; i < playerCount; ++i) {
		std::string username;
		std::cout << "Enter username for Player " << (i + 1) << ": ";
		std::getline(std::cin, username);

		if (!ValidateUsername(username)) {
			std::cout << "Invalid username format\n";
			i--;
			continue;
		}

		auto userPtr = storage.get_pointer<UserData>(username);
		if (!userPtr) {
			UserData newUser{ username, 0, 0, 0 };
			storage.replace(newUser);
			userPtr = storage.get_pointer<UserData>(username);
			if (!userPtr) {
				std::cerr << "Failed to retrieve user data after insertion\n";
				return {};
			}
		}

		auto weapon = std::make_shared<Weapon>(1.0f);
		Player player(username, weapon, { 0, 0 });
		player.SetScore(userPtr->totalScore);
		player.SetPoints(userPtr->totalPoints);
		players.push_back(player);
	}

	return players;
}

//for console test
void LoginSystem::ShowLoginRegisterMenu()
{
	std::cout << "Welcome to Titan Vanguard!" << std::endl;
	std::cout << "1. Log in" << std::endl;
	std::cout << "2. Sign up" << std::endl;
	std::cout << "Choose an option: ";

	int option;
	std::cin >> option;
	std::cin.ignore();

	std::string username;

	std::cout << "Username: ";
	std::getline(std::cin, username);

	if (option == 1)
	{
		if (!LoginUser(username))
		{
			std::cout << "Try again" << std::endl;
		}
		else 
		{
			// Start Game 
			//first check if this function works without game logic
		}
	}
	else if (option == 2)
	{
		if (!RegisterUser(username))
		{
			std::cout << "Try again!" << std::endl;
		}
		else
		{
			// Start game
		}
	}
	else
	{
		std::cout << "Invalid option!" << std::endl;
	}

}

void LoginSystem::Run()
{
		ShowLoginRegisterMenu();
}


