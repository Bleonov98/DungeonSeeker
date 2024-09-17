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
	glm::vec2 leftPoint = GetNearestPoints(left, right).first;
	glm::vec2 rightPoint = GetNearestPoints(left, right).second;
}

std::pair<glm::vec2, glm::vec2> Dungeon::GetNearestPoints(DungeonNode* left, DungeonNode* right)
{
	glm::vec2 pointA = glm::vec2();
	glm::vec2 pointB = glm::vec2();

	return std::pair<glm::vec2, glm::vec2>();
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

	glBindVertexArray(0);
}
#endif
