#ifndef RENDERER_H
#define RENDERER_H

#include "Texture.h"
#include "Shader.h"

#include <vector>

class Renderer
{
public:

	Renderer() { InitRenderData(); }

	void InitRenderData();
	void Draw(Texture texture, std::vector<glm::mat4> instancedMatrices, std::vector<glm::vec3> instancedColours);
	void DrawTexture(Texture texture);

	~Renderer();

private:

	unsigned int VAO, VBO;

};

#endif // !RENDERER_H
