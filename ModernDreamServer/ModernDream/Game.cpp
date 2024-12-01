#include "Game.h"

Game::Game(Map map, std::vector<Player> players)
{
    this->map = map;
    this->players = players;
}

Map Game::GetMap()
{
    return map;
}

std::vector<Player> Game::GetPlayers()
{
    return players;
}

void Game::DetermineWinner()
{

    if (players.empty())
    {
        std::cout << "No players in the game." << std::endl;
        return;
    }
    std::sort(players.begin(), players.end(), [](Player &a, Player &b)
              { return a.GetPoints() > b.GetPoints(); });
    WinGame();
    if (players.size() > 1)
    {
        FinishSecond();
    }
    std::cout << "Game Results:" << std::endl;
    for (auto &player : players)
    {
        std::cout << "Player " << player.GetName() << ": Points = " << player.GetPoints() << ", Score = " << player.GetScore() << std::endl;
    }
}

void Game::WinGame()
{
    std::vector<Player> players = GetPlayers();

    if (players.empty())
    {
        std::cout << "No players in the game!" << std::endl;
        return;
    }

    auto winner = std::max_element(players.begin(), players.end(), [](Player &a,Player &b)
                                  { return a.GetScore() < b.GetScore(); });

    if (winner != players.end())
    {
        winner->SetPoints(winner->GetPoints() + 200);
        winner->SetScore(winner->GetScore() + 2);

        std::cout << "Player " << winner->GetName() << " won the game and recived 200 bonus points and 2 score points. " << "Total points: " << winner->GetPoints() << ", Total score: " << winner->GetScore() << std::endl;
    }
}

void Game::FinishSecond()
{
    std::vector<Player> players =GetPlayers();

    if (players.size() < 2)
    {
        std::cout << "Not enough players to determine who finished second!" << std::endl;
        return;
    }

    auto first = std::max_element(players.begin(), players.end(), [](Player &a, Player &b)
                                  { return a.GetScore() < b.GetScore(); });

    auto second = std::max_element(players.begin(), players.end(), [&](Player &a,Player &b)
                                   {
        if(&a == &(*first)) return true;
        if(&b == &(*first)) return false;
        return a.GetScore() < b.GetScore(); });

    if (second != players.end())
    {
        second->SetScore(second->GetScore() + 1);
        std::cout << "PLayers " << second->GetName() << " finished second and recived 1 score point. Total score: " << second->GetScore() << std::endl;
    }
}

void Game::CheckAndApplyWeaponUpgrade()
{
    for (auto &player : players)
    {
        if (player.GetScore() >= 10 && !player.IsSpeedBoostUsed())
        {
            player.GetWeapon().UpgradeSpeed(1.5f);
            player.GetWeapon().UpgradeWaitingTime(0.5f);
            player.SetSpeedBoostUsed(true);
            std::cout << "Player " << player.GetName() << " has reached 10 score points and upgraded weapon: increased bullet speed and reduced waiting time!" << std::endl;
        }
    }
}

void Game::TriggerBomb(int x, int y)
{
    Wall *wall = map.GetWallAt(x, y);
    if (wall != nullptr && wall->IsDestructible())
    {
        Bomb *bomb = map.GetBombAt(x, y);
        if (bomb != nullptr && !bomb->GetStatus())
        {
            bomb->SetStatus(true);
            std::cout << "A bomb was triggered at (" << x << " , " << y << ")" << std::endl;
        }
        for (Wall &otherWall : map.GetWalls())
        {
            double distance = std::sqrt(std::pow(otherWall.GetPosition().first - x, 2) + std::pow(otherWall.GetPosition().second - y, 2));
            if (distance <= 10.0 && otherWall.IsDestructible())
            {
                otherWall.Destroy();
                map.SetFreePosition(otherWall.GetPosition().first, otherWall.GetPosition().second);
            }
        }
        for (Player &player : players)
        {
            double distance = std::sqrt(std::pow(player.GetPosition().first - x, 2) + std::pow(player.GetPosition().second - y, 2));
            if (distance <= 10.0)
            {
                player.ResetPosition();
                std::cout << "The player " << player.GetName() << "was hit by the explosion\n";
            }
        }
    }
}
