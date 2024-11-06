#include "Player.h"

void Player::Move(MoveDirection dir, float dt)
{
	static MoveDirection lastTextureDir = DIR_RIGHT;

	switch (dir)
	{
	case STAND:
		break;
	case DIR_UP:
		position.y -= speed * dt;
		break;
	case DIR_DOWN:
		position.y += speed * dt;
		break;
	case DIR_LEFT:
		if (lastTextureDir != dir) flipHorizontally = true;
		position.x -= speed * dt;
		break;
	case DIR_RIGHT:
		if (lastTextureDir != dir) flipHorizontally = false;
		position.x += speed * dt;
		break;
	default:
		break;
	}

	if (dir == DIR_LEFT || dir == DIR_RIGHT) lastTextureDir = dir;
}

void Player::UpdateAABB()
{
	hBox.SetBorder(glm::vec2(position.x + size.x / 3.0f, position.y + size.y / 2.5f), position + size - glm::vec2(size.x / 3.0f, 0));
}
