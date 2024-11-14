#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "MagicSphere.h"

class Enemy : public Character
{
public:
	Enemy(glm::vec2 position, glm::vec2 size, float speed) :Character(position, size, speed) {
		hp = 10.0f;
		damage = 5.0f;
	};
	
	void CheckPlayer(glm::vec2 playerPos);
	virtual void Move(glm::vec2 playerPos, float dt);

protected:

	AttackType atkType = PHYSICAL;
	float visionRange = 300.0f;

};

class Skeleton : public Enemy
{
public:
	Skeleton(glm::vec2 position, glm::vec2 size, float speed) : Enemy(position, size, speed) {
		hp = 15.0f;
		armor = 2.5f;
	};
};

class Skull : public Enemy
{
public:
	Skull(glm::vec2 position, glm::vec2 size, float speed) : Enemy(position, size, speed) {
		atkType = MAGICAL;
		hp = 10;
		resist = 5.0f;
	};
	void Move(glm::vec2 playerPos, float dt) override;
private:
	float tpTimer = 0.0f, tpDelay = 2.5f;
};

class Vampire : public Enemy, public std::enable_shared_from_this<Vampire>
{
public:
	Vampire(glm::vec2 position, glm::vec2 size, float speed) : Enemy(position, size, speed) {
		atkType = PHYSICAL;
		armor = 1.0f;
		resist = 3.0f;
	};
	std::shared_ptr<MagicSphere> Attack(glm::vec2 playerPos, float dt);
private:
	float atkTimer = 0.0f, atkDelay = 1.75f;
};

#endif // !ENEMY_H