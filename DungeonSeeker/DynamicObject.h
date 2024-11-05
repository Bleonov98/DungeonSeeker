#ifndef DYNAMIC_H
#define DYNAMIC_H

#include "GameObject.h"
#include "ResourceManager.h"

enum MoveDirection {
	STAND,
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
};

class DynamicObject : public GameObject
{
public:

	DynamicObject(glm::vec2 position, glm::vec2 size, float angle = 0.0f, glm::vec3 color = glm::vec3(1.0f)) : GameObject(position, size, angle, color) {};

	virtual void PlayAnimation() = 0;
	bool AnimationPlayed(float dt);

	MoveDirection GetDirection() { return direction; }
	
	void SetDirection(MoveDirection direction) { this->direction = direction; }

	virtual ~DynamicObject() {}

protected:

	MoveDirection direction = STAND;
	float animationTime = 0.0f, swapTime = 0.4f;
	int frame = 0;
	bool animToggle = false;
};

#endif // !DYNAMIC_H