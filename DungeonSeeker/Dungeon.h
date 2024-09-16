#ifndef DUNGEON_H
#define DUNGEON_H

#include "Grid.h"
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
	glm::vec3 colour;
};

struct Corridor
{
public:

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
};

class Dungeon
{
public:

	Dungeon() {};

	void GenerateDungeon();
	void SplitNode(DungeonNode* root, int step);
	void GenerateRoom(DungeonNode* leaf);
	void ConnectRooms(DungeonNode* left, DungeonNode* right);
	void DrawDungeon();

private:

	DungeonNode* dungeon; // tree root node for algorithm
	std::vector<std::vector<std::shared_ptr<Grid>>> GridVec; // ??

	std::vector<Room> rooms; // for drawing
	unsigned int VAO, VBO;

};
#endif // !DUNGEON_H