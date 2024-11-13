#ifndef CHARACTER_H
#define CHARACTER_H

#include "DynamicObject.h"

enum AttackType {
	PHYSICAL,
	MAGICAL,
	PURE
};

enum ActionState {
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
	float GetSpeed() { return speed; }
	ActionState GetAction() { return aState; }
	AttackType GetAtkType() { return attackType; }

protected:

	AttackType attackType = PHYSICAL;
	ActionState aState = IDLE;

	glm::vec2 pushDirection;
	float pushDistance = 50.0f;

	float speed, damage = 0.0f, armor = 0.0f, resist = 0.0f, hp; // resist - magic resistance
	float damageTime = 0.0f, damageDelay = 1.0f;
	bool damaged = false;

};

#endif // !CHARACTER_H