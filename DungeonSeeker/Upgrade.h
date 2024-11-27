#ifndef UPGRADE_H
#define UPGRADE_H

#include "Item.h"

enum class UpgradeType {
	UpgradeStats,
	UpgradeType
};

class Upgrade : public Item
{
public:
	Upgrade(glm::vec2 position, glm::vec2 size, UpgradeType type) : Item(position, size) {
		if (type == UpgradeType::UpgradeStats) {
			itemID = ItemID::statsUpgrade;
			SetTexture("statScroll");
		}
		else if (type == UpgradeType::UpgradeType)
		{
			itemID = ItemID::typeUpgrade;
			SetTexture("typeScroll");
		}
	};
};

#endif // !UPGRADE_H