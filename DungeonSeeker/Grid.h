#ifndef GRID_H
#define GRID_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Grid
{
public:

	Grid(glm::vec2 cellPosition, int cellData = 0);

	void InitInstanceBuffer(std::vector<glm::mat4> cellMatrices, std::vector<glm::vec3> cellColours);
	void DrawGrid(std::vector<glm::mat4>& cellMatrices, std::vector<glm::vec3> cellColours);

	glm::mat4 GetMatrix() { return cellMatrix; }
	glm::vec3 GetColour() { return cellColour; }
	glm::vec2 GetPosition() { return cellPosition; }
	int GetCellData() { return cellData; }

private:
	
	int cellData = 0;
	glm::mat4 cellMatrix;
	glm::vec3 cellColour;
	glm::vec2 cellPosition, cellSize = glm::vec2(10.0f);
	unsigned int VAO, VBO, colourVBO, matVBO;

};

#endif // !GRID_H