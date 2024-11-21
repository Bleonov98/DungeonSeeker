#ifndef POTION_H
#define POTION_H

#include "Item.h"

enum class PotionSize {
	SMALL,
	BIG
};

class Potion : public Item
{
public:
	Potion(glm::vec2 position, glm::vec2 size, PotionSize pSize) : Item(position, size) {};
	virtual void InitPotionData(std::string texSize) = 0;
protected:
};

class HealthPotion : public Potion
{
public:
	HealthPotion(glm::vec2 position, glm::vec2 size, PotionSize pSize) : Potion(position, size, pSize) {
		std::string texSize;
		if (pSize == PotionSize::SMALL) {
			itemID = ItemID::smallHealthPotion;
			texSize = "Small";
		}
		else {
			itemID = ItemID::healthPotion;
			texSize = "";
		}
		InitPotionData(texSize);
	}
	void InitPotionData(std::string texSize) override;
};


class MSPotion : public Potion
{
public:
	MSPotion(glm::vec2 position, glm::vec2 size, PotionSize pSize) : Potion(position, size, pSize) {
		std::string texSize;
		if (pSize == PotionSize::SMALL) {
			itemID = ItemID::smallMsPotion;
			texSize = "Small";
		}
		else {
			itemID = ItemID::msPotion;
			texSize = "";
		}
		InitPotionData(texSize);
	};
	void InitPotionData(std::string texSize) override;
};

#endif // !POTION_H


