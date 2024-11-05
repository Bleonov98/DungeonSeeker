#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
class Player : public Character
{
public:
	Player(glm::vec2 position, glm::vec2 size, float speed = 10.0f) : Character(position, size, speed) {
		swapTime = 0.35f;
	};

	void Move(MoveDirection dir, float dt);
	void PlayAnimation() override;
	void Hit();

	void UpdateAABB() override;

private:

};

#endif // !PLAYER_H