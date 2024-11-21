#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Enemy.h"
#include "Item.h"
#include "Inventory.h"

class Player : public Character 
{
public:
	Player(glm::vec2 position, glm::vec2 size, float speed) : Character(position, size, speed) {
		maxHealth = hp = 15.0f;
		damage = 10;
		armor = 1.5f;
		resist = 2.5f;
	};

	void Move(MoveDirection dir, float dt);	
	void ProcessAction(float dt);
	
	void Attack();
	bool AttackCollision(std::shared_ptr<Enemy> enemy);

	void LevelUp(float experience);
	void Heal(float healRate);
	void SpeedUp(float speedRate);
	void AddToInventory(std::shared_ptr<Item> item) { inventory.AddItem(item); }
	Inventory& GetInventory() { return this->inventory; }

	void UpdateAABB() override;

	// 
	int GetLvl() { return lvl; }
	float GetExpPercentage() { return exp / expThreshold; }
	float GetHpPercentage() { return hp / maxHealth; }
private:

	// attack
	Inventory inventory;
	AABB daggerhBox;
	float attackDuration = 0.45f, attackTimer = 0.0f;
	
	float exp = 0.0f, expThreshold = 50.0f;
	int lvl = 1;


};

#endif // !PLAYER_H