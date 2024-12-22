export module bomb;
#include <utility>

export using Coordinates = std::pair<int, int>;

export class Bomb
{
private:
	static const int radius = 10;
	Coordinates position;
	bool isActive;

public:
	Bomb(Coordinates position);
	Coordinates GetPosition() const;
	bool GetStatus() const;
	void SetStatus(bool status);
};
