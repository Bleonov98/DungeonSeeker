#include "Dungeon.h"

#include <optional>

Dungeon::Dungeon()
{
#ifdef _TESTING
	float vertices[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
#endif
}

void Dungeon::GenerateDungeon(int width, int height)
{
	this->width = width, this->height = height;
	DungeonNode* dungeon = new DungeonNode(glm::vec2(0.0f), width, height);

	SplitNode(dungeon, 4);
	ConnectRooms(dungeon);
}

void Dungeon::SplitNode(DungeonNode* root, int step)
{
	step--;
	float multiplier = 0.6f - static_cast<float>(step) * 0.05f;

	DungeonNode* left;
	DungeonNode* right;

	int direction = rand() % 2;

	if (direction == 0) {
		left = new DungeonNode(root->position, static_cast<int>(root->width * multiplier), root->height);
		right = new DungeonNode(root->position + glm::vec2(left->width, 0.0f), root->width - left->width, root->height);
	}
	else {
		left = new DungeonNode(root->position, root->width, static_cast<int>(root->height * multiplier));
		right = new DungeonNode(root->position + glm::vec2(0.0f, left->height), root->width, root->height - left->height);
	}

	if (step == 0) {
		GenerateRoom(root);
		
#ifdef _TESTING
		rooms.push_back(*root->room);
#endif
		return;
	}

	SplitNode(left, step);
	SplitNode(right, step);

	root->left = left;
	root->right = right;
}

void Dungeon::GenerateRoom(DungeonNode* leaf)
{
	int randWidth, randHeight, halfX, halfY;

	halfX = std::round(leaf->width / 2);
	halfY = std::round(leaf->height / 2);

	randWidth = halfX + rand() % halfX;
	randHeight = halfY + rand() % halfY;
		
	// for different dungeon sizes, delete when I've done
	int percent = static_cast<int>(width * 0.02f);

	int diff = std::abs(randWidth - randHeight);
	if (randWidth - randHeight >= percent) randWidth -= diff - percent;
	else if (randHeight - randWidth >= percent) randHeight -= diff - percent;

	leaf->room = new Room(leaf->position + glm::vec2(leaf->width, leaf->height) / 2.0f - glm::vec2(randWidth, randHeight) / 2.0f, randWidth, randHeight);
}

void Dungeon::ConnectRooms(DungeonNode* leaf)
{
	if (!leaf->left || !leaf->right) return;

	ConnectRooms(leaf->left);
	ConnectRooms(leaf->right);

	// find mid intersection point
	glm::vec2 midPoint;
	if (leaf->left->position.x != leaf->right->position.x) {
		midPoint = leaf->left->position + glm::vec2(leaf->left->width, leaf->left->height / 2.0f);
	}
	else if (leaf->left->position.y != leaf->right->position.y) {
		midPoint = leaf->left->position + glm::vec2(leaf->left->width / 2.0f, leaf->left->height);
	}

	Room* leftRoom = FindRoomInSubtree(leaf->left, midPoint);
	Room* rightRoom = FindRoomInSubtree(leaf->right, leftRoom->position + glm::vec2(leftRoom->width, leftRoom->height) / 2.0f);

	if (leftRoom && rightRoom) {
		GenerateCorridor(leftRoom, rightRoom);
	}
}

// finding nearest points
std::pair<glm::vec2, glm::vec2> GetNearestPoints(Room* first, Room* second)
{
	std::pair<glm::vec2, glm::vec2> nearestPoints;

	// diff between centres 
	glm::vec2 firstMid = first->position + glm::vec2(first->width, first->height) / 2.0f;
	glm::vec2 secondMid = second->position + glm::vec2(second->width, second->height) / 2.0f;

	glm::vec2 dif = secondMid - firstMid;
	dif = glm::normalize(dif);

	if (std::abs(dif.x) > std::abs(dif.y)) {
		nearestPoints.first = firstMid + dif * (first->width / 2.0f);
		nearestPoints.second = secondMid - dif * (second->width / 2.0f);
	}
	else {
		nearestPoints.first = firstMid + dif * (first->height / 2.0f);
		nearestPoints.second = secondMid - dif * (second->height / 2.0f);
	}

	nearestPoints.first.x = std::max(first->position.x, std::min(nearestPoints.first.x, first->position.x + first->width - 1));
	nearestPoints.first.y = std::max(first->position.y, std::min(nearestPoints.first.y, first->position.y + first->height - 1));

	nearestPoints.second.x = std::max(second->position.x, std::min(nearestPoints.second.x, second->position.x + second->width - 1));
	nearestPoints.second.y = std::max(second->position.y, std::min(nearestPoints.second.y, second->position.y + second->height - 1));

	return nearestPoints;
}

// ray intersecion struct
struct IntersectionResult {
	glm::vec2 point;
};

// finding intersection point
std::optional<IntersectionResult> IntersectRays(glm::vec2 P1, glm::vec2 D1, glm::vec2 P2, glm::vec2 D2) {

	float denominator = D1.x * D2.y - D1.y * D2.x;

	bool isParallel = false;
	if (fabs(denominator) < 1e-6) {
		isParallel = true;
	}

	float t = (P2.x * D2.y - P2.y * D2.x - P1.x * D2.y + P1.y * D2.x) / denominator;
	float s = (P1.x * D1.y - P1.y * D1.x - P2.x * D1.y + P2.y * D1.x) / denominator;

	if (t >= 0 && s >= 0) {
		glm::vec2 intersectionPoint = P1 + t * D1;
		return IntersectionResult{ intersectionPoint };
	}

	return std::nullopt;
}

void Dungeon::GenerateCorridor(Room* first, Room* second)
{
	std::pair<glm::vec2, glm::vec2> nearestPoints = GetNearestPoints(first, second);
	glm::vec2 dif = nearestPoints.second - nearestPoints.first;
	int size = 2;

	int threshold1 = 4;
	int threshold2 = 3;

	if (dif.y == 0) // horizontal corridor
	{
		Corridor cor(nearestPoints.first, std::ceil(dif.x), size);
		corridors.push_back(cor);
	}
	else if (dif.x == 0) { // vertical
		Corridor cor(nearestPoints.first, size, std::ceil(dif.y));
		corridors.push_back(cor);
	}
	else if (dif.x > dif.y && dif.x >= threshold1 && dif.y >= threshold2) { // vertical Z shape
		Corridor cor(nearestPoints.first, std::ceil(dif.x / 2), size);
		corridors.push_back(cor);

		Corridor cor1(nearestPoints.first + glm::vec2(std::ceil(dif.x / 2), 0), size, std::ceil(dif.y));
		corridors.push_back(cor1);

		Corridor cor2(nearestPoints.first + glm::vec2(std::ceil(dif.x / 2), std::ceil(dif.y)), std::ceil(dif.x / 2), size);
		corridors.push_back(cor2);
	}
	else if (dif.y > dif.x && dif.y >= threshold1 && dif.x >= threshold2) { // horizontal Z shape
		Corridor cor(nearestPoints.first, size, std::ceil(dif.y / 2));
		corridors.push_back(cor);

		Corridor cor1(nearestPoints.first + glm::vec2(0, std::ceil(dif.y / 2)), std::ceil(dif.x), size);
		corridors.push_back(cor1);

		Corridor cor2(glm::vec2(nearestPoints.second.x, nearestPoints.first.y + std::ceil(dif.y / 2)), size, std::ceil(dif.y / 2));
		corridors.push_back(cor2);
	}
	else { // L shape
		if (dif.y < 0) {
			Corridor cor(nearestPoints.first + glm::vec2(0, std::ceil(dif.y)), size, std::abs(std::ceil(dif.y)));
			corridors.push_back(cor);
		}
		else {
			Corridor cor(nearestPoints.first, size, std::ceil(dif.y) + size);
			corridors.push_back(cor);
		}

		if (dif.x < 0) {
			Corridor cor1(glm::vec2(nearestPoints.first.x + std::ceil(dif.x), nearestPoints.second.y), std::abs(std::ceil(dif.x)), size);
			corridors.push_back(cor1);
		}
		else {
			Corridor cor1(glm::vec2(nearestPoints.first.x, nearestPoints.second.y), std::ceil(dif.x), size);
			corridors.push_back(cor1);
		}
	}
}

Room* Dungeon::FindRoomInSubtree(DungeonNode* leaf, glm::vec2 midPoint)
{
	if (leaf->IsLeaf()) return leaf->room;

	Room* leftRoom = FindRoomInSubtree(leaf->left, midPoint);
	Room* rightRoom = FindRoomInSubtree(leaf->right, midPoint);	
	
	if (glm::distance(leftRoom->position + glm::vec2(leftRoom->width, leftRoom->height) / 2.0f, midPoint) <=
		glm::distance(rightRoom->position + glm::vec2(rightRoom->width, rightRoom->height) / 2.0f, midPoint)) 
		return leftRoom;
	else return rightRoom;
}

#ifdef _TESTING
void Dungeon::DrawDungeon()
{
	ResourceManager::GetShader("spriteShader").Use();
	ResourceManager::GetShader("spriteShader").SetMatrix4("projection", glm::ortho(0.0f, static_cast<float>(1600), static_cast<float>(900), 0.0f, -1.0f, 1.0f));

	ResourceManager::GetShader("spriteShader").SetBool("menu", false);
	ResourceManager::GetShader("spriteShader").SetBool("test", true);
	ResourceManager::GetShader("spriteShader").SetBool("instanced", false);

	glBindVertexArray(this->VAO);

	for (size_t i = 0; i < rooms.size(); i++)
	{
		ResourceManager::GetShader("spriteShader").SetVector3f("uColour", glm::vec3(0.5f));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(rooms[i].position, 0.0f));
		model = glm::scale(model, glm::vec3(rooms[i].width, rooms[i].height, 0.0f));

		ResourceManager::GetShader("spriteShader").SetMatrix4("uModel", model);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	for (size_t i = 0; i < corridors.size(); i++)
	{
		ResourceManager::GetShader("spriteShader").SetVector3f("uColour", glm::vec3(0.5f));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(corridors[i].position, 0.0f));
		model = glm::scale(model, glm::vec3(corridors[i].length, corridors[i].width, 0.0f));

		ResourceManager::GetShader("spriteShader").SetMatrix4("uModel", model);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindVertexArray(0);
}
#endif