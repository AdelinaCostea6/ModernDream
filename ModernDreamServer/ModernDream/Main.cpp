#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include "Bullet.h"
#include "Wall.h"
#include "Player.h"
#include "Map.h"
#include "Game.h"
#include "LoginSystem.h"
#include "DatabaseManager.h"
#include "Routing.h"
import bomb;
#include "../MapGenerator/MapGenerator/MapGenerator.h"

int main()
{
	std::cout << "===Testing classes===\n\n";

	//bomb test
	std::cout << "--- BOMB TEST ---\n";
	Coordinates bombPos = { 5, 5 };
	Bomb bomb(bombPos);
	std::cout << "Bomb was created at position(" << bomb.GetPosition().first << "," << bomb.GetPosition().second << ")\n";
	std::cout << "Bomb status : " << (bomb.GetStatus() ? "active" : "inactive") << "\n\n";
	
	//bullet test
	std::cout << "--- BULLET TEST ---\n";
	Bullet bullet({ 1, 1 });
	std::cout << "A bullet was created at position: (" << bullet.GetPosition().first
		<< ", " << bullet.GetPosition().second << ")\n";
	std::cout << "Bullet speed: " << bullet.GetSpeed() << "\n";
	bullet.Movement();
	bullet.SetDoubleSpeed();
	std::cout << "Speed after doublation: " << bullet.GetSpeed() << "\n\n";

	//wall test
	std::cout << "---WALL TEST  ---\n";
	Wall wall({ 3, 3 }, WallType::Destructible, 2, true);
	std::cout << "A wall was created at position: (" << wall.GetPosition().first
		<< ", " << wall.GetPosition().second << ")\n";
	std::cout << "Durability: " << wall.GetDurability() << "\n";
	wall.ReduceDurability();
	std::cout << "Durability after reduce: " << wall.GetDurability() << "\n\n";


	// Weapon Test
	std::cout << "--- WEAPON TEST ---\n";
	Weapon weapon(1.0f);
	std::cout << "A weapon was created with the speed: " << weapon.GetSpeed() << "\n";
	std::cout << "Initial waiting time: " << weapon.GetWaitingTime() << "\n";
	weapon.UpgradeSpeed(0.5f);
	weapon.UpgradeWaitingTime(1.0f);
	std::cout << "After upgrade - Speed: " << weapon.GetSpeed()
		<< ", Waiting time: " << weapon.GetWaitingTime() << "\n\n";

    //Map Test
	/*
	std::cout << "--- MAP TEST ---\n";  
	Map gameMap({ 10, 10 });  
	gameMap.GenerateMap(2);  
	std::cout << "Map size: (" << gameMap.GetSize().first  
		<< ", " << gameMap.GetSize().second << ")\n"; 
	std::cout << "Number of walls: " << gameMap.GetWalls().size() << "\n";  
	std::cout << "Number of bombs: " << gameMap.GetBombs().size() << "\n\n";  
    */
	/*
	
	LoginSystem loginSystem;
	std::vector<Player> players = loginSystem.RegisterPlayersForGame();
	loginSystem.UpdatePlayerStats(players); 
	for (auto& player : players) {
		std::cout << "\nTesting movement for " << player.GetName() << ": Use W, A, S, D to move the player (press 'q' to exit)\n";
			if (_kbhit()) {
				char ch = _getch();
				if (ch == 'q') {
					std::cout << "Exiting movement test for " << player.GetName() << "\n";
					break; 
				}
				player.Movement(generatedMap);
			}

		
		
	}
	*/
	/*
	std::cout << "--- PLAYER TEST ---\n";
	auto playerWeapon = std::make_shared<Weapon>(1.0f);
	Player player1("Player1:", playerWeapon, std::make_pair(0, 0));
	std::cout << "Player was created: " << player1.GetName() << "\n";
	std::cout << "Initial position: (" << player1.GetPosition().first
		<< ", " << player1.GetPosition().second << ")\n";
	
	std::cout << "Testing movement: Use W, A, S, D to move the player (press 'q' to exit)\n";
	while (true)
	{
		player1.Movement(map2);
		if (_kbhit() && _getch() == 'q') break;
	}

	player1.Shoot();
	std::cout << "Remaining lifes: " << player1.GetLifes() << "\n\n";

	

	// Game Test
	std::cout << "--- GAME TEST ---\n";
	//std::vector<Player> players;
	auto weapon2 = std::make_shared<Weapon>(1.0f);
	players.push_back(Player("Player1", playerWeapon, std::make_pair(0, 0)));
	players.push_back(Player("Player2", weapon2, std::make_pair(9, 9)));

	

	// Some actions on the game:
	players[0].SetPoints(100);
	players[1].SetPoints(50);
	
	Game game(map2, players);
	std::cout << "Testing winner determination:\n";
	game.DetermineWinner();

	std::cout << "Testing weapon upgrades:\n";
	players[0].SetScore(10);
	game.CheckAndApplyWeaponUpgrade();

	std::cout << "Testing bomb activation:\n";
	game.TriggerBomb(5, 5);*/



	//// Inițializare harta
	//std::cout << "--- MAP GENERATOR TEST ---\n";
	//MapGenerator generator({10, 10}); 
	//generator.GenerateMap(2);         
	//std::cout << "Map generated successfully.\n";

	//// Obținem date brute din MapGenerator
	//auto wallPositions = generator.GetWallPositions();
	//auto bombPositions = generator.GetBombPositions();
	
	
	
	std::cout << "--- PLAYER TEST ---\n";
	auto playerWeapon = std::make_shared<Weapon>(1.0f); 
	Player player1("Player1", playerWeapon, std::make_pair(0, 0));  
	std::cout << "Player was created: " << player1.GetName() << "\n";
	std::cout << "Initial position: (" << player1.GetPosition().first
		<< ", " << player1.GetPosition().second << ")\n";

	// Testare mișcare
	//std::cout << "Testing movement: Use W, A, S, D to move the player (press 'q' to exit)\n";
	//while (true) {
	//	player1.Movement(wallPositions, bombPositions);  // Mișcare pe hartă
	//	if (_kbhit() && _getch() == 'q') break;
	//}

	// Testare tragere
	player1.Shoot();
	std::cout << "Remaining lifes: " << player1.GetLifes() << "\n\n";

	// Testare joc
	std::cout << "--- GAME TEST ---\n";
	// Creăm mai mulți jucători
	auto weapon2 = std::make_shared<Weapon>(1.0f);
	std::vector<Player> players;
	players.push_back(Player("Player1", playerWeapon, std::make_pair(0, 0)));
	players.push_back(Player("Player2", weapon2, std::make_pair(9, 9)));

	// Some actions on the game:
	players[0].SetPoints(100);
	players[1].SetPoints(50);

	//Game game(wallPositions, bombPositions, players);  // Jocul cu harta generatăv   
	//std::cout << "Testing winner determination:\n";
	//game.DetermineWinner();

	//std::cout << "Testing weapon upgrades:\n";
	//players[0].SetScore(10);
	//game.CheckAndApplyWeaponUpgrade();

	//std::cout << "Testing bomb activation:\n";
	//game.TriggerBomb(5, 5);

	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n";

	DatabaseManager dbManager;

	// Crează o instanță a clasei Routing și rulează serverul
	http::Routing routing;
	routing.Run(dbManager);

	return 0;


	

}
