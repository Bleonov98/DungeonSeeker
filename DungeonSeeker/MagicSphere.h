#ifndef MAGICSPHERE_H
#define MAGICSPHERE_H

#include "Character.h"

class MagicSphere : public DynamicObject
{
public:
	MagicSphere(std::shared_ptr<Character> owner, glm::vec2 direction) : DynamicObject(position, size) {
		this->owner = owner;
		this->damage = owner->GetDamage();
		this->position = owner->GetPos() + owner->GetSize() / 2.0f;
		this->size = owner->GetSize() / 2.5f;

		speed = 400.0f;
		this->direction = direction;
	};
	void Move(float dt);
	int GetDamage() { return this->damage; }
	std::shared_ptr<Character> GetOwner() { return owner; }

	~MagicSphere() {};

private:
	glm::vec2 direction;
	int damage;
	float speed;
	std::shared_ptr<Character> owner;
};

#endif // !MAGICSPHERE_H



