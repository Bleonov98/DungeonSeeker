#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Renderer.h"
#include "ResourceManager.h"
#include "AABB.h"

class GameObject
{
public:
	GameObject(glm::vec2 position, glm::vec2 size, float angle = 0.0f, glm::vec3 colour = glm::vec3(1.0f)) {
		this->position = position, this->colour = colour, this->size = size, this->angle = angle;

		hBox.SetBorder(position, position + size);
	};

	// Set
	void SetPos(glm::vec2 position) { this->position = position; }
	void SetSize(glm::vec2 size) { this->size = size; }
	void SetColour(glm::vec3 color) { this->colour = colour; }
	void SetAngle(float angle) { this->angle = angle; }
	void RefreshMatrix();

	void SetTexture(std::string name) { textures.push_back(name); }
	virtual void UpdateAABB() { hBox.SetBorder(position, position + size); }

	// Get
	glm::mat4 GetMatrix() { return objectMatrix; }
	glm::vec2 GetPos() { return position; }
	glm::vec2 GetSize() { return size; }
	glm::vec3 GetColour() { return colour; }
	float GetAngle() { return angle; }
	std::string GetTextureName() { return textures[activeTex]; }

	// collisions
	bool PointCollision(const glm::vec2 point) { return hBox.IntersectPoint(point); }
	bool ObjectCollision(const GameObject& other) { return hBox.Intersects(other.hBox); }
	bool ObjectCollision(const AABB& other) { return hBox.Intersects(other); }

	bool BottomCollision(const GameObject& other) { return hBox.IntersectBottom(other.hBox); }
	bool RightCollision(const GameObject& other) { return hBox.IntersectRight(other.hBox); }
	bool LeftCollision(const GameObject& other) { return hBox.IntersectLeft(other.hBox); }
	bool TopCollision(const GameObject& other) { return hBox.IntersectTop(other.hBox); }

	// - - - - - -
	void DeleteObject() { isDeleted = true; }
	bool IsDeleted() { return isDeleted; }
	virtual ~GameObject() {};

protected:

	std::vector<std::string> textures;
	GLuint activeTex = 0;

	AABB hBox;

	glm::mat4 objectMatrix;
	glm::vec3 colour;
	glm::vec2 position, size;
	bool flipHorizontally = false;
	float angle;

	bool isDeleted = false;
};

#endif // !GAMEOBJECT_H