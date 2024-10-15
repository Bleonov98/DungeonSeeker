#ifndef DUNGEON_H
#define DUNGEON_H

#include "ResourceManager.h"
#include <memory>
#include <vector>
#include <map>

struct Room
{
public:
	Room() {};
	Room(glm::vec2 position, int width, int height) : position(position), width(width), height(height) {};
	int width, height;
	glm::vec2 position;
};

struct Corridor
{
public:
	Corridor(glm::vec2 position, int length, int width) : position(position), length(length), width(width) {};
	int length, width;
	glm::vec2 position;
};

struct DungeonNode
{
public:
	DungeonNode(glm::vec2 position, int width, int height) : position(position), width(width), height(height) {};
	int width, height;
	glm::vec2 position = glm::vec2(0.0f);
	DungeonNode* left = nullptr;
	DungeonNode* right = nullptr;
	Room* room = nullptr;
	bool IsLeaf() const { return left == nullptr && right == nullptr; }
};

class Dungeon
{
public:

	Dungeon();

	void GenerateDungeon();

	std::vector<Room> rooms;
	std::vector<Corridor> corridors;

#ifdef _TESTING
	void DrawDungeon();
#endif

private:

	void SplitNode(DungeonNode* root, int step);
	void GenerateRoom(DungeonNode* leaf);
	void ConnectRooms(DungeonNode* leaf);
	void GenerateCorridor(Room* first, Room* second);
	Room* FindRoomInSubtree(DungeonNode* leaf, glm::vec2 midPoint);

	DungeonNode* dungeon;

#ifdef _TESTING
	unsigned int VAO, VBO;
#endif

};
#endif // !DUNGEON_H