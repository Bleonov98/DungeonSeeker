#include "Enemy.h"

void Enemy::CheckPlayer(glm::vec2 playerPos)
{
	if (glm::distance(this->position, playerPos) <= visionRange)
		aState = ATTACK;
	else
		aState = IDLE;
}

void Enemy::Move(glm::vec2 playerPos, float dt)
{
	glm::vec2 direction = glm::normalize(playerPos - position);
	this->position += direction * speed * dt;

	if (direction.x < 0.0f)
		flipHorizontally = true;
	else
		flipHorizontally = false;
}

void Skull::Move(glm::vec2 playerPos, float dt)
{
	enum TpDirection { TOP, RIGHT, BOT, LEFT };
	TpDirection tpDir = static_cast<TpDirection>(rand() % (LEFT + 1));

	glm::vec2 direction = glm::normalize(playerPos - position);

	// teleport to player
	tpTimer += dt;
	if (tpTimer >= tpDelay) 
	{
		float disValue = 125.0f;
		glm::vec2 distance;

		switch (tpDir)
		{
		case TOP:
			distance = glm::vec2(0.0f, -disValue);
			break;
		case RIGHT:
			distance = glm::vec2(disValue, 0.0f);
			break;
		case BOT:
			distance = glm::vec2(0.0f, disValue);
			break;
		case LEFT:
			distance = glm::vec2(-disValue, 0.0f);
			break;
		default:
			break;
		}

		position = playerPos + distance;
		tpTimer = 0.0f;
	}
	else {
		this->position += direction * speed * dt;
	}

	if (direction.x < 0.0f)
		flipHorizontally = true;
	else
		flipHorizontally = false;
}

std::shared_ptr<MagicSphere> Vampire::Attack(glm::vec2 playerPos, float dt)
{
	atkTimer += dt;
	if (atkTimer >= atkDelay)
	{
		glm::vec2 projectileDir = glm::normalize(playerPos - position);

		MagicSphere projectile(position + size / 2.0f, size / 2.5f, projectileDir, damage);
		for (size_t i = 0; i < 5; i++)
		{
			projectile.SetTexture("fireball" + std::to_string(i));
		}
		projectile.AddAnimation("main", 0, 5, 0.1f, true);

		atkTimer = 0.0f;
		return std::make_shared<MagicSphere>(projectile);
	}
	else return nullptr;
}
