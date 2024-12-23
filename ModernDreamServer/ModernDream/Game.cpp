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
    auto maxPlayer = std::max_element(players.begin(), players.end(), [](const std::unique_ptr<Player>& a, const std::unique_ptr<Player>& b)
        {
            return a->GetScore() < b->GetScore();
        });

    if (maxPlayer != players.end() && *maxPlayer)
    {
        (*maxPlayer)->SetPoints((*maxPlayer)->GetPoints() + 200);
        (*maxPlayer)->SetScore((*maxPlayer)->GetScore() + 2);

        std::cout << "Player " << (*maxPlayer)->GetName() << " won the game and received 200 bonus points and 2 score points. "
            << "Total points: " << (*maxPlayer)->GetPoints() << ", Total score: " << (*maxPlayer)->GetScore() << std::endl;
    }
}



void Game::FinishSecond()
{
    auto first = std::max_element(players.begin(), players.end(), [](const std::unique_ptr<Player>& a, const std::unique_ptr<Player>& b)
        {
            return a->GetScore() < b->GetScore();
        });

    auto second = std::max_element(players.begin(), players.end(), [&](const std::unique_ptr<Player>& a, const std::unique_ptr<Player>& b)
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

    auto bombIt = std::find_if(bombs.begin(), bombs.end(), [&](const std::unique_ptr<Bomb>& bomb) {
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
        if (playerPtr)
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
}
