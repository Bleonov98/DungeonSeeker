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
	Potion(glm::vec2 position, glm::vec2 size, PotionSize pSize) : Item(position, size) {
		this->pSize = pSize;
	};
	virtual void Use() = 0;

	virtual void InitPotionData(std::string texSize) = 0;
	PotionSize GetPotionSize() { return pSize; }
protected:
	PotionSize pSize;
	float duration, durationTime = 0.0f;
};

class HealthPotion : public Potion
{
public:
	HealthPotion(glm::vec2 position, glm::vec2 size, PotionSize pSize) : Potion(position, size, pSize) {
		itemID = ItemID::HP_POTION;
		duration = 10.0f;

		std::string texSize;
		if (pSize == SMALL) {
			hpRestore = 0.75f;
			texSize = "Small";
		}
		else {
			hpRestore = 1.5f;
			texSize = "";
		}
		InitPotionData(texSize);
	}

	void InitPotionData(std::string texSize) override;
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

		std::string texSize;
		if (pSize == SMALL) {
			msUpRate = 25.0f;
			texSize = "Small";
		}
		else {
			msUpRate = 50.0f;
			texSize = "";
		}
		InitPotionData(texSize);
	};

	void InitPotionData(std::string texSize) override;
	void Use() override;

private:
	float msUpRate;
};

#endif // !POTION_H


