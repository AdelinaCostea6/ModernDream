module bomb;

Bomb::Bomb(Coordinates position)
	: position(position), isActive(true) {}

Coordinates Bomb::GetPosition() const
{
	return position;
}

bool Bomb::GetStatus() const
{
	return isActive;
}

void Bomb::SetStatus(bool status)
{
	isActive = status;
}
