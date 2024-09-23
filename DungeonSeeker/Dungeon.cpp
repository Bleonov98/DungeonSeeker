#include "Dungeon.h"

void Dungeon::GenerateDungeon()
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
	// DungeonNode* dungeon = new DungeonNode(glm::vec2(0.0f), 6400, 3600);
	DungeonNode* dungeon = new DungeonNode(glm::vec2(0.0f), 1600, 900);

	SplitNode(dungeon, 4);
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

	ConnectRooms(left, right);
}

void Dungeon::GenerateRoom(DungeonNode* leaf)
{
	int randWidth, randHeight, halfX, halfY;

	halfX = leaf->width / 2;
	halfY = leaf->height / 2;

	randWidth = halfX + rand() % (halfX - 5);
	randHeight = halfY + rand() % (halfY - 5);
		
	int diff = abs(randWidth - randHeight);
	if (randWidth - randHeight >= 40) randWidth -= diff - 40;
	else if (randHeight - randWidth >= 40) randHeight -= diff - 40;

	leaf->room = new Room(leaf->position + glm::vec2(leaf->width, leaf->height) / 2.0f - glm::vec2(randWidth, randHeight) / 2.0f, randWidth, randHeight);
}

void Dungeon::ConnectRooms(DungeonNode* left, DungeonNode* right)
{
	if (!left->room || !right->room) return;

	std::pair<glm::vec2, glm::vec2> nearestPoints = GetNearestPoints(left, right);
	glm::vec2 leftPoint = nearestPoints.first;
	glm::vec2 rightPoint = nearestPoints.second;

	int minLength = 2, minWidth = 2;

	int length = rightPoint.x - leftPoint.x;
	int width = rightPoint.y - leftPoint.y;

	if (length < minLength) length = minLength;
	if (width < minWidth) width = minWidth;

	Corridor corr(leftPoint, length, width);
	corridors.push_back(corr);
}

// making vectors of points for every surface
std::vector<glm::vec2> GetSurfacePointsVector(DungeonNode* node) 
{
	std::vector<glm::vec2> points;

	for (size_t y = 0; y < node->room->height; y++)
	{
		for (size_t x = 0; x < node->room->width; x++)
		{
			glm::vec2 point = glm::vec2(node->room->position.x + x, node->room->position.y + y);
			points.push_back(point);
		}
	}

	return points;
}

// finding minimal points between two surfaces
std::pair<glm::vec2, glm::vec2> BruteForce(std::vector<glm::vec2> objectA, std::vector<glm::vec2> objectB)
{
	float minDistance = FLT_MAX;
	glm::vec2 closestA, closestB;

	for (auto A : objectA)
	{
		for (auto B : objectB)
		{
			float distance = glm::distance(A, B);
			if (minDistance > distance) {
				minDistance = distance;
				closestA = A;
				closestB = B;
			}
		}
	}

	return std::make_pair(closestA, closestB);
}

// divide and conquer algorithm for finding closest pair of points
std::pair<glm::vec2, glm::vec2> DivideFind(std::vector<glm::vec2> objectA, std::vector<glm::vec2> objectB)
{
	if (objectA.empty() || objectB.empty()) {
		std::cout << "ERROR: no points vectors to find" << std::endl << "Dungeon.cpp, line 135;" << std::endl;
		return std::make_pair(glm::vec2(0, 0), glm::vec2(0, 0));
	}
	 
	if (objectA.size() <= 3 || objectB.size() <= 3)
		return BruteForce(objectA, objectB);

	std::vector<glm::vec2> firstHalfA(objectA.begin(), objectA.begin() + objectA.size() / 2);
	std::vector<glm::vec2> secondHalfA(objectA.begin() + objectA.size() / 2, objectA.end());

	std::vector<glm::vec2> firstHalfB(objectB.begin(), objectB.begin() + objectB.size() / 2);
	std::vector<glm::vec2> secondHalfB(objectB.begin() + objectB.size() / 2, objectB.end());

	std::pair<glm::vec2, glm::vec2> firstDivided = DivideFind(firstHalfA, firstHalfB);
	std::pair<glm::vec2, glm::vec2> secondDivided = DivideFind(secondHalfA, secondHalfB);

	float firstDistance = glm::distance(firstDivided.first, firstDivided.second);
	float secondDistance = glm::distance(secondDivided.first, secondDivided.second);

	float minDistance = std::min(firstDistance, secondDistance);
	std::vector<glm::vec2> stripA;
	std::vector<glm::vec2> stripB;

	for (const auto& point : objectA) {
		if (abs(point.x - objectA[objectA.size() / 2].x) < minDistance)
			stripA.push_back(point);
	}

	for (const auto& point : objectB) {
		if (abs(point.x - objectB[objectB.size() / 2].x) < minDistance)
			stripB.push_back(point);
	}

}

std::pair<glm::vec2, glm::vec2> Dungeon::GetNearestPoints(DungeonNode* left, DungeonNode* right)
{
	std::vector<glm::vec2> surfaceA = GetSurfacePointsVector(left);
	std::vector<glm::vec2> surfaceB = GetSurfacePointsVector(right);

	return DivideFind(surfaceA, surfaceB);
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
		ResourceManager::GetShader("spriteShader").SetVector3f("uColour", glm::vec3(0.05f * i, 0.01f * i, 0.2f * i));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(rooms[i].position, 0.0f));
		model = glm::scale(model, glm::vec3(rooms[i].width, rooms[i].height, 0.0f));

		ResourceManager::GetShader("spriteShader").SetMatrix4("uModel", model);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	for (size_t i = 0; i < corridors.size(); i++)
	{
		ResourceManager::GetShader("spriteShader").SetVector3f("uColour", glm::vec3(1.0f));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(corridors[i].position, 0.0f));
		model = glm::scale(model, glm::vec3(corridors[i].length, corridors[i].width, 0.0f));

		ResourceManager::GetShader("spriteShader").SetMatrix4("uModel", model);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindVertexArray(0);
}
#endif