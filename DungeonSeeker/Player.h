#ifndef PLAYER_H
#define PLAYER_H

enum ActionState {
	IDLE,
	ATTACK
};

#include "Character.h"
#include "Enemy.h"

class Player : public Character
{
public:
	Player(glm::vec2 position, glm::vec2 size, float speed) : Character(position, size, speed) {
		damage = 10;
		armor = 1.5f;
		resist = 1.5f;
	};

	void Move(MoveDirection dir, float dt);
	
	void ProcessAction(float dt);
	
	void Attack();
	bool AttackCollision(std::shared_ptr<Enemy> enemy);

	ActionState GetAction() { return aState; }

	void UpdateAABB() override;

private:

	// attack
	AABB daggerhBox;
	float attackDuration = 0.45f, attackTimer = 0.0f;
	
	ActionState aState = IDLE;

};

#endif // !PLAYER_H