#include "Game.h"
/*
Game::Game(Map map, std::vector<Player> players)
{
    this->map = map;
    this->players = players;
}
*/



Game::Game(std::vector<std::unique_ptr<Wall>> walls, std::array<std::unique_ptr<Bomb>, 3> bombs, std::array<std::unique_ptr<Player>, 4> players)
    :walls(std::move(walls)), bombs(std::move(bombs)), players(std::move(players))
{
}

const Map& Game::GetMap() const
{
    return map;
}

std::array<std::unique_ptr<Player>, 4>& Game::GetPlayers()
{
    return players;
}

std::vector<std::unique_ptr<Wall>>& Game::GetWalls()
{
    return walls;
}

const std::array<std::unique_ptr<Bomb>, 3>& Game::GetBombs() const
{
    return bombs;
}


void Game::DetermineWinner()
{
    
    if (std::all_of(players.begin(), players.end(), [](const std::unique_ptr<Player>& player) { return !player; }))
    {
        std::cout << "No players in the game." << std::endl;
        return;
    }

    std::vector<Player*> sortedPlayers;
    for (const auto& player : players)
    {
        if (player)
        {
            sortedPlayers.push_back(player.get());
        }
    }

    std::sort(sortedPlayers.begin(), sortedPlayers.end(), [](const Player* a, const Player* b)
        {
            return a->GetPoints() > b->GetPoints();
        });

    WinGame();
    if (sortedPlayers.size() > 1)
    {
        FinishSecond();
    }

    std::cout << "Game Results:" << std::endl;
    for (const auto* player : sortedPlayers)
    {
        std::cout << "Player " << player->GetName()
            << ": Points = " << player->GetPoints()
            << ", Score = " << player->GetScore() << std::endl;
    }
}

void Game::WinGame()
{
    auto maxPlayer = std::max_element(players.begin(), players.end(),
        [](const std::unique_ptr<Player>& a, const std::unique_ptr<Player>& b)
        {
            return a ? a->GetScore() < (b ? b->GetScore() : 0) : false;
        });

    if (maxPlayer != players.end() && *maxPlayer)
    {
        (*maxPlayer)->SetPoints((*maxPlayer)->GetPoints() + 200);
        (*maxPlayer)->SetScore((*maxPlayer)->GetScore() + 2);

        std::cout << "Player " << (*maxPlayer)->GetName()
            << " won the game and received 200 bonus points and 2 score points. "
            << "Total points: " << (*maxPlayer)->GetPoints()
            << ", Total score: " << (*maxPlayer)->GetScore() << std::endl;
    }
} 
void Game::FinishSecond()
{
    auto first = std::max_element(players.begin(), players.end(),
        [](const std::unique_ptr<Player>& a, const std::unique_ptr<Player>& b)
        {
            return a ? a->GetScore() < (b ? b->GetScore() : 0) : false;
        });

    auto second = std::max_element(players.begin(), players.end(),
        [&](const std::unique_ptr<Player>& a, const std::unique_ptr<Player>& b)
        {
            if (a == *first) return true;
            if (b == *first) return false;

            return a ? a->GetScore() < (b ? b->GetScore() : 0) : false;
        });

    if (second != players.end() && *second && *second != *first)
    {
        (*second)->SetScore((*second)->GetScore() + 1);
        std::cout << "Player " << (*second)->GetName()
            << " finished second and received 1 score point. "
            << "Total score: " << (*second)->GetScore() << std::endl;
    }
}
void Game::CheckAndApplyWeaponUpgrade()
{
    for (auto& playerPtr : players)
    {
        if (playerPtr && playerPtr->GetScore() >= 10 && !playerPtr->IsSpeedBoostUsed())
        {
            playerPtr->GetWeapon().UpgradeSpeed(1.5f);
            playerPtr->GetWeapon().UpgradeWaitingTime(0.5f);
            playerPtr->SetSpeedBoostUsed(true);

            std::cout << "Player " << playerPtr->GetName()
                << " has reached 10 score points and upgraded their weapon: "
                << "increased bullet speed and reduced waiting time!" << std::endl;
        }
    }
}
void Game::TriggerBomb(int x, int y)
{
    Wall* wall = map.GetWallAt(x, y);
    if (!wall || !wall->IsDestructible())
    {
        std::cout << "No destructible wall at (" << x << ", " << y << ")" << std::endl;
        return;
    }


    auto bombIt = std::find_if(bombs.begin(), bombs.end(),
        [&](const std::unique_ptr<Bomb>& bomb) {
            return bomb && bomb->GetPosition() == Coordinates{ x, y };
        });


    if (bombIt == bombs.end() || (*bombIt)->GetStatus())
    {
        std::cout << "No valid bomb found at (" << x << ", " << y << ")" << std::endl;
        return;
    }


    (*bombIt)->SetStatus(true);
    std::cout << "A bomb was triggered at (" << x << ", " << y << ")" << std::endl;


    for (const auto& wallPtr : walls)
    {
        if (wallPtr && wallPtr->IsDestructible())
        {
            double distance = std::sqrt(std::pow(wallPtr->GetPosition().first - x, 2) +
                std::pow(wallPtr->GetPosition().second - y, 2));
            if (distance <= 10.0)
            {
                wallPtr->Destroy();
                map.SetFreePosition(wallPtr->GetPosition().first, wallPtr->GetPosition().second);
                std::cout << "Wall at (" << wallPtr->GetPosition().first
                    << ", " << wallPtr->GetPosition().second << ") destroyed" << std::endl;
            }
        }
    }


    for (const auto& playerPtr : players)
    {
        if (playerPtr)
        {
            double distance = std::sqrt(std::pow(playerPtr->GetPosition().first - x, 2) +
                std::pow(playerPtr->GetPosition().second - y, 2));
            if (distance <= 10.0)
            {
                playerPtr->ResetPosition();
                std::cout << "Player " << playerPtr->GetName() << " was hit by the explosion" << std::endl;
            }
        }
    }
}


void Game::GenerateMap(int numPlayers) {
    MapGenerator generator;
    generator.GenerateMap(numPlayers);
    map = Map(generator);  

    
    auto startPositions = map.GetPlayerStartPositions();

    int i = 0;
    for (auto& playerPtr : players) {
        if (playerPtr && i < startPositions.size()) {
            playerPtr->SetPosition(startPositions[i]);  
            std::cout << "Jucătorul " << playerPtr->GetName() << " este plasat la ("
                << startPositions[i].first << ", " << startPositions[i].second << ")\n";
            ++i;
        }
    }
}


void Game::ShootBullet(const Player& player) {
    Bullet newBullet(player.GetPosition(), player.GetDirection());
    bullets.push_back(newBullet);  // Adaugă bullet la coadă
    std::cout << "Jucătorul " << player.GetName() << " a tras un bullet la poziția: ("
        << newBullet.GetPosition().first << ", " << newBullet.GetPosition().second << ")\n";
}
void Game::UpdateBullets() {
    int mapHeight = map.GetHeight();
    int mapWidth = map.GetWidth();
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        it->Movement(mapHeight, mapWidth);

        if (!it->GetIsActive()) {
            std::cout << "Bullet out of bounds at: (" << it->GetPosition().first << ", " << it->GetPosition().second << ")";
            it = bullets.erase(it);  // Șterge glonțul care a ieșit
        }
        else {
            std::cout << "Bullet moved to: (" << it->GetPosition().first << ", " << it->GetPosition().second << ")";
            ++it;  // Continuă iterarea
        }
        /*if (it->CheckCollisionWithPlayers(players) || it->CheckCollisionwithWalls(walls)) {
            it->SetIsInactive();
            it = bullets.erase(it);
        }
        else {
            ++it;
            std::cout << "Bullet declansat la pozitia " << it->GetPosition().first << " " << it->GetPosition().second << "\n";
        }*/
    }
}

//void Game::UpdateBullets() {
//    for (auto it = bullets.begin(); it != bullets.end();) {
//        it->Movement(map.GetHeight(), map.GetWidth());
//
//        if (it->CheckCollisionWithPlayers(players)) {
//           // qDebug() << "Bullet hit a player!";
//            it = bullets.erase(it);  // Ștergere glonț
//            continue;
//        }
//
//        if (it->CheckCollisionwithWalls(walls)) {
//           // qDebug() << "Bullet hit a wall!";
//            it = bullets.erase(it);
//            continue;
//        }
//
//        auto [x, y] = it->GetPosition();
//        if (x < 0 || y < 0 || x >= map.GetWidth() || y >= map.GetHeight()) {
//           // qDebug() << "Bullet out of bounds!";
//            it = bullets.erase(it);
//        }
//        else {
//            ++it;
//        }
//    }
//}



const std::deque<Bullet>& Game::GetBullets() const {
    return bullets; 
}

