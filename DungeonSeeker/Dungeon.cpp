#include "Dungeon.h"

void Dungeon::GenerateDungeon()
{
	Room rootRoom(glm::vec2(0.0f), 200, 120);

	SplitRoom(rootRoom, 4);
}

void Dungeon::SplitRoom(Room room, int step)
{
	step--;
	float multiplier = 0.6f - static_cast<float>(step) * 0.05f;
	
	Room first;
	Room second;

	int direction = rand() % 10; // 0-4 for vertical split, 5-9 for horizontal
	
	if (direction <= 4) {
		first.InitRoom(room.GetPosition(), static_cast<int>(room.GetWidth() * multiplier), room.GetHeight());
		second.InitRoom(room.GetPosition() + glm::vec2(first.GetWidth(), 0.0f), room.GetWidth() - first.GetWidth(), room.GetHeight());
	}
	else {
		first.InitRoom(room.GetPosition(), room.GetWidth(), static_cast<int>(room.GetHeight() * multiplier));
		second.InitRoom(room.GetPosition() + glm::vec2(0.0f, first.GetHeight()), room.GetWidth(), room.GetHeight() - first.GetHeight());
	}

	if (step == 0) {
		dungeon.push_back(first);
		dungeon.push_back(second);
		
		return;
	}

	SplitRoom(first, step);
	SplitRoom(second, step);
}

void Room::InitRoom(glm::vec2 position, int width, int height)
{
	this->position = position;
	this->width = width;
	this->height = height;
}
