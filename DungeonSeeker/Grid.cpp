#include "Grid.h"

Grid::Grid(glm::vec2 cellPosition, int cellData)
{
	this->cellPosition = cellPosition;

	float vertices[] = {
		// pos
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	cellMatrix = glm::mat4(1.0f);
	cellMatrix = glm::translate(cellMatrix, glm::vec3(cellPosition, 0.0f));
}

void Grid::InitInstanceBuffer(std::vector<glm::mat4> cellMatrices, std::vector<glm::vec3> cellColours)
{
	glBindVertexArray(VAO);

	// colour instance buffer
	glGenBuffers(1, &colourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cellColours.size(), &cellColours.data()[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glVertexAttribDivisor(1, 1);

	// matrix instance buffer
	glGenBuffers(1, &matVBO);
	glBindBuffer(GL_ARRAY_BUFFER, matVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * cellMatrices.size(), &cellMatrices[0], GL_STATIC_DRAW);

	// instance matrix
	std::size_t vec4Size = sizeof(glm::vec4);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glBindVertexArray(0);
}

void Grid::DrawGrid(std::vector<glm::mat4>& cellMatrices, std::vector<glm::vec3> cellColours)
{
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, colourVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * cellColours.size(), &cellColours.data()[0]);

	glBindBuffer(GL_ARRAY_BUFFER, matVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * cellMatrices.size(), &cellMatrices.data()[0]);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, cellMatrices.size());
	glBindVertexArray(0);
}