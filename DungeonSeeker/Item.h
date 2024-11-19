#ifndef ITEM_H
#define ITEM_H

enum class ItemID {
	HP_POTION = 0,
	MS_POTION = 1,
	STAT_UPGRADE = 10,
	TYPE_UPGRADE = 11
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

	virtual void Use() = 0;
	void Hide() { hidden = true; }

protected:
	ItemID itemID;
	bool hidden = false;
};

struct DropEntry
{
	ItemID itemID;
	int dropChance = 0; // 0 - 100%
};

#endif // !ITEM_H