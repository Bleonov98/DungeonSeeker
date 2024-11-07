#include "Animator.h"

bool Animator::AnimationPlayed(Animation animation, float dt)
{
	animationTime += dt;
	if (animationTime >= animation.swapTime) {
		animationTime = 0.0f;
		return true;
	}
	return false;
}