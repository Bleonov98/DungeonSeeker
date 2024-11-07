#ifndef ANIMATOR_H
#define ANIMATOR_H

struct Animation {
public:
	Animation() {};
	Animation(unsigned int startIndex, unsigned int totalFrames, float swapTime) : startIndex(startIndex), totalFrames(totalFrames), swapTime(swapTime), currentIndex(startIndex){};
	unsigned int startIndex;
	unsigned int currentIndex;
	unsigned int totalFrames;
	float swapTime = 0.4f;
};

class Animator
{
public:
	Animator() {};

	bool AnimationPlayed(Animation animation, float dt);

	float animationTime = 0.0f;

};

#endif // !ANIMATOR_H