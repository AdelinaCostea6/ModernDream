#pragma once
#include <cstdint>
#include <optional>
#include <utility>
import bomb;

enum class WallType : std::uint8_t 
{
    NonDestructible = 0,
    Destructible = 1
};

class Wall {
private:
    std::pair<std::uint16_t, std::uint16_t> position;
    WallType type;
    std::uint16_t durability; 
    bool destructible;
    std::optional<Bomb> bomb; 

public:
    Wall(std::pair<std::uint16_t, std::uint16_t> position, WallType type, std::uint16_t durability, bool destructible, std::optional<Bomb> bomb = std::nullopt);

    WallType GetType() const noexcept;
    const std::pair<std::uint16_t, std::uint16_t>& GetPosition() const noexcept;
    std::uint16_t GetDurability() const noexcept;
    bool GetDestructible() const noexcept;
    void ReduceDurability() noexcept;
    bool IsDestructible() const noexcept;
    void Destroy() noexcept;
};
