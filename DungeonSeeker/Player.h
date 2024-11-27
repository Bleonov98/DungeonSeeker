#ifndef PLAYER_H
#define PLAYER_H

#include <functional>

#include "Character.h"
#include "Enemy.h"
#include "Potion.h"
#include "Upgrade.h"
#include "Inventory.h"

enum class Effects {
	none,
	smallSpeedUpEffect,
	speedUpEffect
};

class Player : public Character 
{
public:
	Player(glm::vec2 position, glm::vec2 size, float speed) : Character(position, size, speed) {
		baseSpeed = speed;
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
	bool UseItem(int num);
	void AddToInventory(std::shared_ptr<Item> item) { inventory.AddItem(item); }
	Inventory& GetInventory() { return this->inventory; }

	void UpdateAABB() override;

	// 
	int GetLvl() { return lvl; }
	float GetExpPercentage() { return exp / expThreshold; }
	float GetHpPercentage() { return hp / maxHealth; }

	Effects GetEffect() { return effect; } 
	float GetEffectDuration() { return effectDuration; }

private:

	// effects
	Effects effect;

	void ApplySmallHealthPotion();
	void ApplyHealthPotion();
	
	float speedUpEffectDuration = 0.0f, baseSpeed, effectDuration = 0.0f;
	void ApplySmallMsPotion();
	void ApplyMsPotion();
	
	void ApplyStatsUpgrade();
	void ApplyTypeUpgrade() { attackType = PURE; }

	std::unordered_map<ItemID, std::function<void()>> itemHandlers = {
		{ItemID::smallHealthPotion, [this]() { ApplySmallHealthPotion(); }},
		{ItemID::healthPotion, [this]() { ApplyHealthPotion(); }},
		{ItemID::smallMsPotion, [this]() { ApplySmallMsPotion(); }},
		{ItemID::msPotion, [this]() { ApplyMsPotion(); }},
		{ItemID::statsUpgrade, [this]() { ApplyStatsUpgrade(); }},
		{ItemID::typeUpgrade, [this]() { ApplyTypeUpgrade(); }}
	};

	// attack
	AABB daggerhBox;
	float attackDuration = 0.35f, attackTimer = 0.0f;
	
	// stats
	Inventory inventory;
	float exp = 0.0f, expThreshold = 50.0f;
	int lvl = 1;
};

#endif // !PLAYER_H