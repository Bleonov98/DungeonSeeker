#include "Potion.h"

void HealthPotion::InitPotionData(std::string texSize)
{
	for (size_t i = 0; i < 4; i++)
	{
		SetTexture("hpPotion" + texSize + std::to_string(i));
	}
	AddAnimation("main", 0, 4, 0.35f, true);
}

void MSPotion::InitPotionData(std::string texSize)
{
	for (size_t i = 0; i < 4; i++)
	{
		SetTexture("msPotion" + texSize + std::to_string(i));
	}
	AddAnimation("main", 0, 4, 0.35f, true);
}