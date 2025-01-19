#include <iostream>
#include <conio.h>
#include <thread>
#include <vector> 
#include <memory>
#include <chrono>
#include "Bullet.h"
#include "Wall.h"
#include "Player.h"
#include "Map.h"
#include "Game.h"
#include "DatabaseManager.h"
#include "Routing.h"
import bomb;
#include "../MapGenerator/MapGenerator/MapGenerator.h"

int main() {

    DatabaseManager dbManager;
    http::Routing routing;
    routing.Run(dbManager);
    return 0;
}

