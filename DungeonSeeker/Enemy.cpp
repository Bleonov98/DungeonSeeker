#include "Enemy.h"

void Enemy::CheckPlayer(glm::vec2 playerPos)
{
	if (glm::distance(this->position, playerPos) <= visionRange)
		aState = ATTACK;
}

void Enemy::Move(glm::vec2 playerPos)
{

}
