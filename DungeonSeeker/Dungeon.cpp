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

	// find mid intersection point
	glm::vec2 midPoint;
	if (leaf->left->position.x != leaf->right->position.x) {
		midPoint = leaf->left->position + glm::vec2(leaf->left->width, leaf->left->height / 2.0f);
	}
	else if (leaf->left->position.y != leaf->right->position.y) {
		midPoint = leaf->left->position + glm::vec2(leaf->left->width / 2.0f, leaf->left->height);
	}

	Room* leftRoom = FindRoomInSubtree(leaf->left, midPoint);
	Room* rightRoom = FindRoomInSubtree(leaf->right, midPoint);

	if (leftRoom && rightRoom) {
		GenerateCorridor(leftRoom, rightRoom);
	}
}

// making side vector
std::vector<glm::vec2> GetSide(Room* room, int sideNum)
{
	std::vector<glm::vec2> side;
	switch (sideNum)
	{
	case 0:
		for (size_t i = 0; i < room->height; i++)
		{
			side.push_back(glm::vec2(room->position.x, room->position.y + i));
		}
		break;
	case 1:
		for (size_t i = 0; i < room->width; i++)
		{
			side.push_back(glm::vec2(room->position.x + i, room->position.y));
		}
		break;
	case 2:
		for (size_t i = 0; i < room->height; i++)
		{
			side.push_back(glm::vec2(room->position.x + room->width, room->position.y + i));
		}
		break;
	case 3:
		for (size_t i = 0; i < room->width; i++)
		{
			side.push_back(glm::vec2(room->position.x + i, room->position.y + room->height));
		}
		break;
	default:
		break;
	}

	return side;
}

// finding nearest sides
std::vector<std::vector<glm::vec2>> GetNearestSides(Room* left, Room* right)
{
	std::vector<std::vector<glm::vec2>> sides;

	// sides [0, 1, 2, 3] = left,top,right,bot;
	std::vector<glm::vec2> leftSides{ glm::vec2(left->position.x, left->position.y + left->height / 2.0f), glm::vec2(left->position.x + left->width / 2.0f, left->position.y), 
		glm::vec2(left->position.x + left->width, left->position.y + left->height / 2.0f), glm::vec2(left->position.x + left->width / 2.0f, left->position.y + left->height) };
	std::vector<glm::vec2> rightSides{ glm::vec2(right->position.x, right->position.y + right->height / 2.0f), glm::vec2(right->position.x + right->width / 2.0f, right->position.y),
		glm::vec2(right->position.x + right->width, right->position.y + right->height / 2.0f), glm::vec2(right->position.x + right->width / 2.0f, right->position.y + right->height) };

	std::pair<int, int> cnt;
	float minValue = FLT_MAX;
	for (size_t i = 0; i < leftSides.size(); i++)
	{
		for (size_t j = 0; j < rightSides.size(); j++)
		{
			float distance = glm::distance(leftSides[i], rightSides[j]);
			if (distance < minValue) {
				minValue = distance;
				cnt.first = i;
				cnt.second = j;
			}
		}
	}

	sides.push_back(GetSide(left, cnt.first));
	sides.push_back(GetSide(right, cnt.second));

	return sides;
}

// finding nearest points
std::pair<glm::vec2, glm::vec2> GetNearestPoints(std::vector<glm::vec2> first, std::vector<glm::vec2> second)
{
	std::pair<glm::vec2, glm::vec2> nearestPoints = std::make_pair(first[first.size() / 2], second[second.size() / 2]);
	float minValue = glm::distance(nearestPoints.first, nearestPoints.second);

	std::vector<std::pair<glm::vec2, glm::vec2>> samePoints;

	for (size_t i = 0; i < first.size(); i++)
	{
		for (size_t j = 0; j < second.size(); j++)
		{
			float distance = glm::distance(first[i], second[j]);
			if (distance < minValue) {
				minValue = distance;
				nearestPoints.first = first[i];
				nearestPoints.second = second[j];

				samePoints.clear();
				samePoints.push_back(nearestPoints);
			}
			else if (distance == minValue) {
				samePoints.push_back(std::make_pair(first[i], second[j]));
			}
		}
	}

	if (!samePoints.empty()) nearestPoints = samePoints[samePoints.size() / 2];

	return nearestPoints;
}

void Dungeon::GenerateCorridor(Room* first, Room* second)
{
	std::vector<std::vector<glm::vec2>> sides = GetNearestSides(first, second);
	std::pair<glm::vec2, glm::vec2> nearestPoints = GetNearestPoints(sides[0], sides[1]);

	if ((first->position.x >= second->position.x && first->position.x <= second->position.x + second->width) ||
		(second->position.x >= first->position.x && second->position.x <= first->position.x + first->width)  || 
		(first->position.y >= second->position.y && first->position.y <= second->position.y + second->height) ||
		(second->position.y >= first->position.y && second->position.y <= first->position.y + first->height)) // straight corridor
	{
		int length, width;
		length = nearestPoints.second.x - nearestPoints.first.x;
		width = nearestPoints.second.y - nearestPoints.first.y;

		if (width <= 0) width = 5;
		else if (length <= 0) length = 5;

		Corridor cor(nearestPoints.first, length, width);
		corridors.push_back(cor);
	}
	else { // Z shaped

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