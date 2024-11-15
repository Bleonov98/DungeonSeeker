#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Enemy.h"
#include "Item.h"
 
class Player : public Character 
{
public:
	Player(glm::vec2 position, glm::vec2 size, float speed) : Character(position, size, speed) {
		damage = 10;
		armor = 1.5f;
		resist = 2.5f;
	};

	void Move(MoveDirection dir, float dt);
	
	void ProcessAction(float dt);
	
	void Attack();
	bool AttackCollision(std::shared_ptr<Enemy> enemy);

	void LevelUp(float experience);

	void UpdateAABB() override;

private:

	// attack
	AABB daggerhBox;
	float attackDuration = 0.45f, attackTimer = 0.0f;
	
	float exp = 0.0f, expThreshold = 50.0f;
	int lvl;

	std::vector<std::shared_ptr<Item>> inventory;

};

#endif // !PLAYER_H