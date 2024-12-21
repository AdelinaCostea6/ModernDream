#include "Bomb.h"
#include <utility>

Bomb::Bomb(Coordinates position)
	: position(std::move(position)), isActive(true) {}

[[nodiscard]] constexpr Coordinates Bomb::GetPositions() const noexcept
{
	return position;
}

[[nodiscard]] constexpr bool Bomb::GetStatus() const noexcept
{
	return isActive;
}

void Bomb::SetStatus(bool status) noexcept
{
	isActive = status;
}
