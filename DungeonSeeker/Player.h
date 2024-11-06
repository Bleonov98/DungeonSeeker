#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
class Player : public Character
{
public:
	Player(glm::vec2 position, glm::vec2 size, float speed) : Character(position, size, speed) {
		swapTime = 0.35f;
		damage = 10;
		armor = 1.5f;
		resist = 1.5f;
	};

	void Move(MoveDirection dir, float dt);

	void UpdateAABB() override;

private:

};

#endif // !PLAYER_H