#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"

class Enemy : public Character
{
public:
	Enemy(glm::vec2 position, glm::vec2 size, float speed) :Character(position, size, speed) {};

	virtual void Move(glm::vec2 playerPos);

private:

};

#endif // !ENEMY_H