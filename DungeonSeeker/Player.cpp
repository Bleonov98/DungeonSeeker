#include "Player.h"

void Player::Move(MoveDirection dir, float dt)
{
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
		position.x -= speed * dt;
		break;
	case DIR_RIGHT:
		position.x += speed * dt;
		break;
	default:
		break;
	}
}

void Player::PlayAnimation()
{
}

void Player::Hit()
{
}
