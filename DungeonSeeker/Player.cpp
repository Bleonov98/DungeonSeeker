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

	if (speedUpEffectDuration > 0.0f) {
		speedUpEffectDuration -= dt;
		effectDuration = speedUpEffectDuration;
	}
	else {
		speedUpEffectDuration = 0.0f;

		speed = baseSpeed;
		effect = Effects::none;
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

void Player::UseItem(int keyIndex)
{
	int invIndex = 0;
	for (auto i : inventory.GetItems())
	{
		if (invIndex == keyIndex)
		{
			if (inventory.RemoveItem(i.first)) 
			{
				itemHandlers.find(i.first)->second();
			}
			break;
		}
		invIndex++;
	}
}

void Player::UpdateAABB()
{
	hBox.SetBorder(glm::vec2(position.x + size.x / 3.0f, position.y + size.y / 2.5f), position + size - glm::vec2(size.x / 3.0f, 0));
	if (flipHorizontally) daggerhBox.SetBorder(glm::vec2(position.x, position.y + size.y / 2.5f), position + glm::vec2(size.x / 2.0f, size.y));
	else daggerhBox.SetBorder(glm::vec2(position.x + size.x / 2.0f, position.y + size.y / 2.5f), position + size);
}

// effects
void Player::ApplySmallHealthPotion() {
	hp += maxHealth * 0.35f;
	if (hp > maxHealth) hp = maxHealth;
}

void Player::ApplyHealthPotion()
{
	hp += maxHealth * 0.75f;
	if (hp > maxHealth) hp = maxHealth;
}

void Player::ApplySmallMsPotion()
{
	if (effect != Effects::smallSpeedUpEffect) 
	{
		speedUpEffectDuration = 0.0f;
		speed = baseSpeed;
		speed += 50.0f;
		effect = Effects::smallSpeedUpEffect;
	}
	speedUpEffectDuration += 40.0f;
}

void Player::ApplyMsPotion()
{
	if (effect != Effects::speedUpEffect)
	{
		speedUpEffectDuration = 0.0f;
		speed = baseSpeed;
		speed += 100.0f;
		effect = Effects::speedUpEffect;
	}
	speedUpEffectDuration += 60.0f;
}

void Player::ApplyStatsUpgrade()
{
	hp = maxHealth += 1.0f;
	resist += 0.5f;
	armor += 0.5f;
	damage += 1.0f;
}
