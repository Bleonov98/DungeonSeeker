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
};

class Dungeon
{
public:

	Dungeon() {};

	void GenerateDungeon();
	void SplitNode(DungeonNode* root, int step);
	void GenerateRoom(DungeonNode* leaf);
	void ConnectRooms(DungeonNode* left, DungeonNode* right);

	std::pair<glm::vec2, glm::vec2> GetNearestPoints(DungeonNode* left, DungeonNode* right);

#ifdef _TESTING
	void DrawDungeon();
#endif

private:

	DungeonNode* dungeon; // tree root node for algorithm
	std::vector<std::vector<std::shared_ptr<Grid>>> GridVec; // ??

#ifdef _TESTING
	std::vector<Room> rooms; // for drawing
	std::vector<Corridor> corridors; // for drawing
	unsigned int VAO, VBO;
#endif

};
#endif // !DUNGEON_H