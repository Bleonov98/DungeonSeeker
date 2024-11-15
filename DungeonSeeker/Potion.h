#ifndef POTION_H
#define POTION_H

#include "Item.h"

enum PotionSize {
	SMALL,
	BIG
};

class Potion : public Item
{
public:
	Potion(glm::vec2 position, glm::vec2 size, PotionSize pSize) : Item(position, size) {};
	virtual void Use() = 0;
protected:
	float duration, durationTime = 0.0f;
};

class HealthPotion : public Potion
{
public:
	HealthPotion(glm::vec2 position, glm::vec2 size, PotionSize pSize) : Potion(position, size, pSize) {
		itemID = ItemID::HP_POTION;
		duration = 10.0f;
		pSize == SMALL ? hpRestore = 0.75f : hpRestore = 1.5f;
	}
	void Use() override;
private:
	float hpRestore, tickRate = 1.0f;
};


class MSPotion : public Potion
{
public:
	MSPotion(glm::vec2 position, glm::vec2 size, PotionSize pSize) : Potion(position, size, pSize) {
		itemID = ItemID::HP_POTION;
		duration = 30.0f;
		pSize == SMALL ? msUpRate = 25.0f : msUpRate = 50.0f;
	};
	void Use() override;
private:
	float msUpRate;
};

#endif // !POTION_H


