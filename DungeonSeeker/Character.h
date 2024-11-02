#ifndef CHARACTER_H
#define CHARACTER_H

#include "DynamicObject.h"
class Character : public DynamicObject
{
public:
	Character(glm::vec2 position, glm::vec2 size, float speed = 10.0f) : DynamicObject(position, size, speed) {};
	
	virtual void Hit() = 0;
	virtual void PlayAnimation() = 0;

	void Death() { this->isDead = true; }
	bool IsDead() { return this->isDead; }

private:

	bool isDead = false;

};

#endif // !CHARACTER_H