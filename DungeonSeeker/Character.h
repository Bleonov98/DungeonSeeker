#ifndef CHARACTER_H
#define CHARACTER_H

#include "DynamicObject.h"

enum AttackType {
	PHYSICAL,
	MAGICAL,
	PURE
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

	void Death() { this->isDead = true; }
	bool IsDead() { return this->isDead; }	

protected:

	AttackType attackType = PHYSICAL;
	float speed, damage, armor, resist, hp; // resist - magic resistance
	bool isDead = false;

};

#endif // !CHARACTER_H