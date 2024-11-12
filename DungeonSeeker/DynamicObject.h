#ifndef DYNAMIC_H
#define DYNAMIC_H

#include "GameObject.h"
#include "ResourceManager.h"
#include "Animator.h"

#include <unordered_map>

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

	// animations
	void AddAnimation(std::string animationName, unsigned int startIndex, unsigned int totalFrames, float swapTime, bool setActive = false);
	void PlayAnimation(float dt);

	// movement
	MoveDirection GetDirection() { return mDirection; }
	void SetDirection(MoveDirection direction) { this->mDirection = direction; }

	virtual ~DynamicObject() {}

protected:

	MoveDirection mDirection = STAND;
	Animator animator;
	std::unordered_map<std::string, Animation> animations;
	std::string currentAnimationName;

};

#endif // !DYNAMIC_H