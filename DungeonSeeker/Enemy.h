#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "MagicSphere.h"
#include "Item.h"

class Enemy : public Character
{
public:
	Enemy(glm::vec2 position, glm::vec2 size, float speed, std::vector<DropEntry> drop) :Character(position, size, speed) {
		maxHealth = hp = 10.0f * statsMultiplier;
		damage = 5.0f * statsMultiplier;
		this->drop = drop;
	};
	
	void CheckPlayer(glm::vec2 playerPos);
	virtual void Move(glm::vec2 playerPos, float dt);

	float GetExpCost() { return expCost; }
	std::vector<std::shared_ptr<Item>> GetLoot();

	// only when +lvl
	inline static float statsMultiplier = 1.0f;

protected:

	std::vector<DropEntry> drop;
	AttackType atkType = PHYSICAL;
	float visionRange = 300.0f, expCost = 25.0f;

};

class Skeleton : public Enemy
{
public:
	Skeleton(glm::vec2 position, glm::vec2 size, float speed, std::vector<DropEntry> drop) : Enemy(position, size, speed, drop) {
		maxHealth = hp = 15.0f * statsMultiplier;
		armor = 2.5f * statsMultiplier;
	};
};

class Skull : public Enemy
{
public:
	Skull(glm::vec2 position, glm::vec2 size, float speed, std::vector<DropEntry> drop) : Enemy(position, size, speed, drop) {
		atkType = MAGICAL;
		maxHealth = hp = 10 * statsMultiplier;
		resist = 5.0f * statsMultiplier;
	};
	void Move(glm::vec2 playerPos, float dt) override;
private:
	float tpTimer = 1.0f, tpDelay = 1.5f;
};

class Vampire : public Enemy, public std::enable_shared_from_this<Vampire>
{
public:
	Vampire(glm::vec2 position, glm::vec2 size, float speed, std::vector<DropEntry> drop) : Enemy(position, size, speed, drop) {
		damage = 7.5f * statsMultiplier;
		atkType = PHYSICAL;
		armor = 1.0f * statsMultiplier;
		resist = 3.0f * statsMultiplier;
		expCost = 50.0f * statsMultiplier;
	};
	std::shared_ptr<MagicSphere> Attack(glm::vec2 playerPos, float dt);
private:
	float atkTimer = 0.0f, atkDelay = 1.35f;
};

#endif // !ENEMY_H