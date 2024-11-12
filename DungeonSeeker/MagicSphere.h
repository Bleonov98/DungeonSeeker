#ifndef MAGICSPHERE_H
#define MAGICSPHERE_H

#include "DynamicObject.h"
class MagicSphere : public DynamicObject
{
public:
	MagicSphere(glm::vec2 position, glm::vec2 size, glm::vec2 direction) : DynamicObject(position, size) {
		speed = 400.0f;
		this->direction = direction;
	};
	void Move(float dt) { position += direction * speed * dt; }
private:
	glm::vec2 direction;
	float speed;
};

#endif // !MAGICSPHERE_H



