#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "Player.h"

class Enemy : public Character
{
public:

	void PlayAnimation() override;

	virtual void Move(glm::vec2 playerPos);

private:

};

#endif // !ENEMY_H