#include "Dungeon.h"

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

	GenerateDungeon();
}

void Dungeon::GenerateDungeon()
{
	DungeonNode* dungeon = new DungeonNode(glm::vec2(0.0f), 1600, 900);

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

	halfX = leaf->width / 2;
	halfY = leaf->height / 2;

	randWidth = halfX + rand() % (halfX - 5);
	randHeight = halfY + rand() % (halfY - 5);
		
	int diff = abs(randWidth - randHeight);
	if (randWidth - randHeight >= 40) randWidth -= diff - 40;
	else if (randHeight - randWidth >= 40) randHeight -= diff - 40;

	leaf->room = new Room(leaf->position + glm::vec2(leaf->width, leaf->height) / 2.0f - glm::vec2(randWidth, randHeight) / 2.0f, randWidth, randHeight);
}

void Dungeon::ConnectRooms(DungeonNode* leaf)
{
	if (!leaf->left || !leaf->right) return;

	ConnectRooms(leaf->left);
	ConnectRooms(leaf->right);

	Room* leftRoom = FindRoomInSubtree(leaf->left);
	Room* rightRoom = FindRoomInSubtree(leaf->right);

	if (leftRoom && rightRoom) {
		GenerateCorridor(leftRoom, rightRoom);
	}
}

void Dungeon::GenerateCorridor(Room* first, Room* second)
{
	glm::vec2 center1(first->position.x + first->width / 2, first->position.y + first->height / 2);
	glm::vec2 center2(second->position.x + second->width / 2, second->position.y + second->height / 2);


}

Room* Dungeon::FindRoomInSubtree(DungeonNode* leaf)
{

	if (leaf->IsLeaf()) return leaf->room;
	if (leaf->left) return FindRoomInSubtree(leaf->left);
	if (leaf->right) return FindRoomInSubtree(leaf->right);
	return nullptr;
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