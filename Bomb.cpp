#include "Bomb.h"


Bomb::Bomb(Coordinates position)
	:position(position), isActive(true) {}

Coordinates Bomb::GetPosition() const
{
	return position;
}

bool Bomb::GetStatus() const
{
	return isActive;
}

//void Bomb::triggerBomb(int width, int height, const std::vector<Player*>& players, std::vector<Wall*> walls)
//{
//    if (!isActive) return;
//    int startX = std::max(0, position.first - radius);
//    int endX = std::min(width - 1, position.first + radius);
//    int startY = std::max(0, position.second - radius);
//    int endY = std::min(height - 1, position.second + radius);
//
//    for (int x = startX; x <= endX; x++)
//        {
//            for (int y = startY; y <= endY; y++)
//            {
//                int dx = x - position.first;
//                int dy = y - position.second;
//                if (dx * dx + dy * dy <= radius * radius)
//                {
//                    for (auto& player : players)
//                    {
//                        if (player->getPosition() == std::make_pair(x, y))
//                        {
//                            player->resetPosition();
//                            std::cout << "Player at (" << x << ", " << y << ") has been hit\\n";
//                        }
//                    }
//
//                    for (auto& wall : walls)
//                    {
//                        if (wall->getPosition() == std::make_pair(x, y))
//                        {
//                            if (wall->isDestructible())
//                            {
//                                wall->destroy();
//                                std::cout << "Wall at (" << x << ", " << y << ") has been destroyed\\n";
//                            }
//                        }
//                    }
//                }
//            }
//        }
//
//    isActive = false;
//
//}
//
