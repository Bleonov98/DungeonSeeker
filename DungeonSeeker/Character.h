#ifndef CHARACTER_H
#define CHARACTER_H

#include "DynamicObject.h"

enum AttackType {
	PHYSICAL,
	MAGICAL,
	PURE
};

enum class ActionState {
	IDLE,
	ATTACK
};

class Character : public DynamicObject
{
public:
	Character(glm::vec2 position, glm::vec2 size, float speed) : DynamicObject(position, size, speed) {
		this->speed = speed;
	};

	// basic
	bool ProcessCollision(GameObject& other, bool first, float dt);

	//
	void Hit(float damage, AttackType type);
	void Push(glm::vec2 position);
	void DamageAnimation(float dt);

	// 
	int GetDamage() { return this->damage; }
	int GetHP() { return hp; }
	int GetArmor() { return armor; }
	int GetResist() { return resist; }
	float GetSpeed() { return speed; }

	ActionState GetAction() { return aState; }
	AttackType GetAtkType() { return attackType; }

	bool IsDamaged() { return damaged; }
	bool IsDead() { return isDead; }

protected:

	AttackType attackType = PHYSICAL;
	ActionState aState = ActionState::IDLE;

	glm::vec2 pushDirection;
	float pushStrength = 200.0f;

	float speed, damage = 0.0f, armor = 0.0f, resist = 0.0f, hp, maxHealth; // resist - magic resistance
	float damageTime = 0.0f, damageDelay = 1.0f;
	bool damaged = false, isDead = false;

};

#endif // !CHARACTER_H