#include "Player.h"

void Player::Move(MoveDirection dir, float dt)
{
	if (aState != IDLE) return;
	
	static MoveDirection lastTextureDir = DIR_RIGHT;

	switch (dir)
	{
	case STAND:
		break;
	case DIR_UP:
		if (position.y > 1.0f) position.y -= speed * dt;
		break;
	case DIR_DOWN:
		position.y += speed * dt;
		break;
	case DIR_LEFT:
		if (lastTextureDir != dir) flipHorizontally = true;
		if (position.x > 1.0f) position.x -= speed * dt;
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

void Player::ProcessAction(float dt)
{
	if (aState == ATTACK) {
		attackTimer += dt;
		if (attackTimer >= attackDuration) {
			aState = IDLE;
			currentAnimationName = "main";
			attackTimer = 0.0f;
		}
	}
}

void Player::Attack()
{
	if (aState != IDLE) return;

	aState = ATTACK;
	currentAnimationName = "attack";
}

bool Player::AttackCollision(std::shared_ptr<Enemy> enemy)
{
	if (enemy->ObjectCollision(daggerhBox)) 
		return true;
	return false;
}

void Player::LevelUp(float experience)
{
	this->exp += experience;
	if (exp >= expThreshold) {
		lvl++;
		exp -= expThreshold;

		hp = maxHealth += 1.0f;
		damage += 2.5f;
		armor += 1.0f;
		resist += 0.5f;

		expThreshold += 25.0f;
	}
}

void Player::UpdateAABB()
{
	hBox.SetBorder(glm::vec2(position.x + size.x / 3.0f, position.y + size.y / 2.5f), position + size - glm::vec2(size.x / 3.0f, 0));
	if (flipHorizontally) daggerhBox.SetBorder(glm::vec2(position.x, position.y + size.y / 2.5f), position + glm::vec2(size.x / 2.0f, size.y));
	else daggerhBox.SetBorder(glm::vec2(position.x + size.x / 2.0f, position.y + size.y / 2.5f), position + size);
}