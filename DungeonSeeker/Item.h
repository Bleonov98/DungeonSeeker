#ifndef ITEM_H
#define ITEM_H

enum class ItemID {
	smallHealthPotion = 0,
	healthPotion = 1,
	smallMsPotion = 2,
	msPotion = 3,
	statsUpgrade = 10,
	typeUpgrade = 11
};

#include "DynamicObject.h"

enum class ItemRarity {
	COMMON,
	RARE,
	UNIQUE
};

class Item : public DynamicObject
{
public:
	Item(glm::vec2 position, glm::vec2 size) : DynamicObject(position, size) {};
	ItemID GetID() { return itemID; }
protected:
	ItemID itemID;
};

struct DropEntry
{
	ItemID itemID;
	int dropChance = 0; // 0 - 100%
};

#endif // !ITEM_H