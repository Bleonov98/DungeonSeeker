#ifndef DUNGEON_H
#define DUNGEON_H

#include "Grid.h"
#include <memory>
#include <vector>
#include <map>

class Room;
class Corridor;

class Dungeon
{
public:

	Dungeon() {};

	void GenerateDungeon();
	void SplitRoom(Room room, int step);

private:

	std::vector<Room> dungeon;
	std::vector<std::vector<std::shared_ptr<Grid>>> GridVec;

};

class Room
{
public:

	Room() {};

	Room(glm::vec2 position, int width, int height) {
		this->position = position;
		this->width = width;
		this->height = height;
	};

	void InitRoom(glm::vec2 position, int width, int height);

	int GetWidth() { return width; }
	int GetHeight() { return height; }
	glm::vec2 GetPosition() { return position; }

private:

	int width, height;
	glm::vec2 position;

};

class Corridor 
{
public:

private:

};
#endif // !DUNGEON_H