#ifndef UPGRADE_H
#define UPGRADE_H

#include "Item.h"

class Upgrade : public Item
{
public:
	Upgrade(glm::vec2 position, glm::vec2 size) : Item(position, size) {};
	virtual void Use() = 0;
protected:

};

class StatsUpgrade : public Upgrade 
{
public:
	StatsUpgrade(glm::vec2 position, glm::vec2 size) : Upgrade(position, size) {
		itemID = ItemID::STAT_UPGRADE;
	};
	void Use() override;
private:

};

class DamageTypeUpgrade : public Upgrade 
{
public:

	DamageTypeUpgrade(glm::vec2 position, glm::vec2 size) : Upgrade(position, size) {
		itemID = ItemID::TYPE_UPGRADE;
	};
	void Use() override;
private:
	
};

#endif // !UPGRADE_H