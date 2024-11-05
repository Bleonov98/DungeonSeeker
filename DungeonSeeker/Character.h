#ifndef CHARACTER_H
#define CHARACTER_H

#include "DynamicObject.h"

class Character : public DynamicObject
{
public:
	Character(glm::vec2 position, glm::vec2 size, float speed = 0.0f) : DynamicObject(position, size, speed) {
		this->speed = speed;
	};

	virtual void PlayAnimation() = 0;

	bool ProcessCollision(GameObject& other, bool first, float dt);

	float GetSpeed() { return speed; }

	void Death() { this->isDead = true; }
	bool IsDead() { return this->isDead; }	

protected:

	float speed;
	bool isDead = false;

};

#endif // !CHARACTER_H