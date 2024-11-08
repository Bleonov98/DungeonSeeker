#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"

class Enemy : public Character
{
public:
	Enemy(glm::vec2 position, glm::vec2 size, float speed) :Character(position, size, speed) {};
	
	void CheckPlayer(glm::vec2 playerPos);
	virtual void Move(glm::vec2 playerPos);

private:

	AttackType atkType = PHYSICAL;
	float visionRange = 50.0f;

};

struct Skeleton : public Enemy
{
public:
	Skeleton(glm::vec2 position, glm::vec2 size, float speed) : Enemy(position, size, speed) {};
};

struct Skull : public Enemy 
{
public:
	Skull(glm::vec2 position, glm::vec2 size, float speed) : Enemy(position, size, speed) {};
};

struct Vampire : public Enemy
{
public:
	Vampire(glm::vec2 position, glm::vec2 size, float speed) : Enemy(position, size, speed) {};
};

#endif // !ENEMY_H