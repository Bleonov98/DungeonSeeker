#include "DynamicObject.h"

bool DynamicObject::AnimationPlayed(float dt)
{
	animationTime += dt;
	if (animationTime >= swapTime) {
		animationTime = 0.0f;
		return true;
	}
	return false;
}
