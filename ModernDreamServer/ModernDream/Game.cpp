#include "Game.h"
/*
Game::Game(Map map, std::vector<Player> players)
{
    this->map = map;
    this->players = players;
}
*/
//Game::Game(std::vector<std::shared_ptr<Wall>> walls,
//    std::vector<std::shared_ptr<Bomb>> bombs,
//    std::vector<std::shared_ptr<Player>> players)
//{
//    this->walls = std::move(walls);
//    this->bombs = { std::make_shared<Bomb>(*bombs[0]), std::make_shared<Bomb>(*bombs[1]), std::make_shared<Bomb>(*bombs[2]) };
//    for (size_t i = 0; i < players.size(); ++i)
//    {
//        this->players[i] = players[i]; 
//    }
//}



const Map& Game::GetMap() const
{
    return map;
}

std::vector<std::shared_ptr<Player>> Game::GetPlayers() const
{
    return { players.begin(), players.end() };
}

const std::vector<std::shared_ptr<Wall>>& Game::GetWalls() const
{
    return walls; 
}

std::vector<std::shared_ptr<Bomb>> Game::GetBombs() const
{
    return { bombs.begin(), bombs.end() }; 
}

void Game::DetermineWinner()
{
    if (players.empty())
    {
        std::cout << "No players in the game." << std::endl;
        return;
    }
    std::sort(players.begin(), players.end(), [](const std::shared_ptr<Player>& a, const std::shared_ptr<Player>& b)
        {
            return a->GetPoints() > b->GetPoints(); 
        });

    WinGame();
    if (players.size() > 1)
    {
        FinishSecond();
    }

    std::cout << "Game Results:" << std::endl;
    for (const auto& player : players)
    {
        std::cout << "Player " << player->GetName()
            << ": Points = " << player->GetPoints()
            << ", Score = " << player->GetScore() << std::endl;
    }
}


void Game::WinGame()
{
    std::vector<std::shared_ptr<Player>> players = GetPlayers(); 

    if (players.empty())
    {
        std::cout << "No players in the game!" << std::endl;
        return;
    }

    auto winner = std::max_element(players.begin(), players.end(), [](const std::shared_ptr<Player>& a, const std::shared_ptr<Player>& b)
        {
            return a->GetScore() < b->GetScore(); 
        });

    if (winner != players.end() && *winner)
    {
        (*winner)->SetPoints((*winner)->GetPoints() + 200);
        (*winner)->SetScore((*winner)->GetScore() + 2);

        std::cout << "Player " << (*winner)->GetName() << " won the game and received 200 bonus points and 2 score points. "
            << "Total points: " << (*winner)->GetPoints() << ", Total score: " << (*winner)->GetScore() << std::endl;
    }
}


void Game::FinishSecond()
{
    std::vector<std::shared_ptr<Player>> players = GetPlayers(); 

    if (players.size() < 2)
    {
        std::cout << "Not enough players to determine who finished second!" << std::endl;
        return;
    }

    auto first = std::max_element(players.begin(), players.end(), [](const std::shared_ptr<Player>& a, const std::shared_ptr<Player>& b)
        {
            return a->GetScore() < b->GetScore(); 
        });

    auto second = std::max_element(players.begin(), players.end(), [&](const std::shared_ptr<Player>& a, const std::shared_ptr<Player>& b)
        {
            if (a == *first) return true; 
            if (b == *first) return false;
            return a->GetScore() < b->GetScore();
        });

    if (second != players.end() && *second)
    {
        (*second)->SetScore((*second)->GetScore() + 1);
        std::cout << "Player " << (*second)->GetName() << " finished second and received 1 score point. Total score: " << (*second)->GetScore() << std::endl;
    }
}


void Game::CheckAndApplyWeaponUpgrade()
{
    for (auto& playerPtr : players)
    {
        if (playerPtr->GetScore() >= 10 && !playerPtr->IsSpeedBoostUsed())
        {
            playerPtr->GetWeapon().UpgradeSpeed(1.5f);
            playerPtr->GetWeapon().UpgradeWaitingTime(0.5f);
            playerPtr->SetSpeedBoostUsed(true);
            std::cout << "Player " << playerPtr->GetName() << " has reached 10 score points and upgraded their weapon: increased bullet speed and reduced waiting time!" << std::endl;
        }
    }
}



void Game::TriggerBomb(int x, int y)
{
    Wall* wall = map.GetWallAt(x, y);
    if (wall == nullptr || !wall->IsDestructible())
    {
        return; 
    }

    auto bombIt = std::find_if(bombs.begin(), bombs.end(), [&](const std::shared_ptr<Bomb>& bomb) {
        return bomb && bomb->GetPosition() == Coordinates{ x, y };
        });

    if (bombIt == bombs.end() || (*bombIt)->GetStatus())
    {
        return; 
    }


    (*bombIt)->SetStatus(true);
    std::cout << "A bomb was triggered at (" << x << " , " << y << ")" << std::endl;

    for (const auto& wallPtr : walls)
    {
        double distance = std::sqrt(std::pow(wallPtr->GetPosition().first - x, 2) +
            std::pow(wallPtr->GetPosition().second - y, 2));
        if (distance <= 10.0 && wallPtr->IsDestructible())
        {
            wallPtr->Destroy();
            map.SetFreePosition(wallPtr->GetPosition().first, wallPtr->GetPosition().second);
        }
    }

    for (const auto& playerPtr : players)
    {
        double distance = std::sqrt(std::pow(playerPtr->GetPosition().first - x, 2) +
            std::pow(playerPtr->GetPosition().second - y, 2));
        if (distance <= 10.0)
        {
            playerPtr->ResetPosition();
            std::cout << "The player " << playerPtr->GetName() << " was hit by the explosion\n";
        }
    }
}

