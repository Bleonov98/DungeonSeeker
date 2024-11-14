#include "MagicSphere.h"

void MagicSphere::Move(float dt)
{
	position += direction * speed * dt;
	if (owner && owner->IsDead()) owner.reset();
}
