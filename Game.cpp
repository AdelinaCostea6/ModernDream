#include "Game.h"

Game::Game(Map map, std::vector<Player> players, std::vector<Bomb> bombs)
{
    this->map = map;
    this->players = players;
    this->bombs = bombs;
}

Map Game::GetMap()
{
    return map;
}

std::vector<Player> Game::GetPlayers()
{
    return players;
}

std::vector<Bomb> Game::GetBombs()
{
    return bombs;
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
    WinGame(&players[0]);
    if (players.size() > 1)
    {
        FinishSecond(&players[1]);
    }
    std::cout << "Game Results:" << std::endl;
    for (auto &player : players)
    {
        std::cout << "Player " << player.GetName() << ": Points = " << player.GetPoints() << ", Score = " << player.GetScore() << std::endl;
    }
}

void Game::WinGame(Player *player)
{
    player->SetPoints(player->GetPoints() + 200);
    player->SetScore(player->GetScore() + 2);
    std::cout << "Player " << player->GetName() << " won the game and received 200 bonus points and 2 score points. Total points: " << player->GetPoints() << ", Total score: " << player->GetScore() << std::endl;
}

void Game::FinishSecond(Player *player)
{
    player->SetScore(player->GetScore() + 1);
    std::cout << "Player " << player->GetName() << " finished second and received 1 score point. Total score: " << player->GetScore() << std::endl;
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
