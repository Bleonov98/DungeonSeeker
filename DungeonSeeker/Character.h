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

	// 
	int GetDamage() { return this->damage; }
	float GetSpeed() { return speed; }
	ActionState GetAction() { return aState; }

	//
	void Death() { this->isDead = true; }
	bool IsDead() { return this->isDead; }	

protected:

	AttackType attackType = PHYSICAL;
	ActionState aState = IDLE;
	float speed, damage = 0.0f, armor = 0.0f, resist = 0.0f, hp; // resist - magic resistance
	bool isDead = false;

};

#endif // !CHARACTER_H