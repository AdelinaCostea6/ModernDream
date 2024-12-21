#pragma once
#include <iostream>
#include <utility>

using Coordinates = std::pair<int, int>;

class Bomb
{
private:
	static constexpr int radius = 10;
	Coordinates position;
	bool isActive;

public:
	explicit Bomb(Coordinates position);
	[[nodiscard]] constexpr Coordinates GetPosition() const noexcept;
	[[nodiscard]] constexpr bool GetStatus() const noexcept;
	void SetStatus(bool status) noexcept;
};
