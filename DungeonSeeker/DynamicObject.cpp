#include "DynamicObject.h"

void DynamicObject::AddAnimation(std::string animationName, unsigned int startIndex, unsigned int totalFrames, float swapTime, bool setActive)
{
    Animation animation(startIndex, totalFrames, swapTime);
    animations.emplace(animationName, animation);
    if (setActive) currentAnimationName = animationName;
}

void DynamicObject::PlayAnimation(float dt)
{
    if (animator.AnimationPlayed(animations[currentAnimationName], dt)) {
        unsigned int maxIndex = animations[currentAnimationName].startIndex + animations[currentAnimationName].totalFrames;
        if (animations[currentAnimationName].currentIndex == maxIndex) animations[currentAnimationName].currentIndex = animations[currentAnimationName].startIndex;
        activeTex = animations[currentAnimationName].currentIndex;
        animations[currentAnimationName].currentIndex++;
    }
}
