#include "GameObject.h"

void GameObject::RefreshMatrix()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 1.0f));
	model = glm::scale(model, glm::vec3(size, 0.0f));
	model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 0.0f)); // ?? which angle
	objectMatrix = model;
}
