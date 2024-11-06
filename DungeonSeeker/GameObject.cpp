#include "GameObject.h"

void GameObject::RefreshMatrix()
{
	glm::mat4 model = glm::mat4(1.0f);
	if (flipHorizontally) {
		model = glm::translate(model, glm::vec3(position.x + size.x, position.y, 0.0f));
		model = glm::scale(model, glm::vec3(size.x * -1.0f, size.y, 0.0f));
	}
	else {
		model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
		model = glm::scale(model, glm::vec3(size.x, size.y, 0.0f));
	}

	objectMatrix = model;
}
