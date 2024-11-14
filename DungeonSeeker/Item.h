#ifndef ITEM_H
#define ITEM_H

#include "DynamicObject.h"
class Item : public DynamicObject
{
public:

	Item(glm::vec2 position, glm::vec2 size) : DynamicObject(position, size) {};

protected:

};


enum PotionType {
	HP,
	SPEED
};

class Potion : public Item 
{
public:
	Potion(glm::vec2 position, glm::vec2 size) : Item(position, size) {};

	PotionType GetPotionType() { return type; }

private:
	PotionType type;
};

enum ScrollType {
	ATK,
	DEF,
	DMGTYPE
};

class Scroll : public Item 
{
public:
	Scroll(glm::vec2 position, glm::vec2 size) : Item(position, size) {};

	ScrollType GetScrollType() { return type; }

private:
	ScrollType type;
};

#endif // !ITEM_H