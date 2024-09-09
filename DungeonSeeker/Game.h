#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "TextRenderer.h"
#include "ResourceManager.h"
#include "GameObject.h"

enum GameState {
	MENU,
	ACTIVE,
	PAUSED
};

class Game
{
public:

	Game(const int width, const int height) {
		this->width = width, this->height = height;
	}

	// basics
	void Init();
	void LoadResources();
	void InitObjects();

	void ProcessInput(float dt);
	void Update(float dt);

	void Render();
	template <typename T>
	void DrawObject(std::vector<T*> objectVector);
	//void DrawStats();

	void Menu();
	//void Restart();

	// game

	// pub vars
	bool Keys[1024], KeysProcessed[1024], mouseKeys[8], mouseKeysProcessed[8], close = false;
	float xMouse, yMouse;

	~Game();

private:

	glm::mat4 projection;

	std::vector<TextRenderer*> textButtons;
	std::vector<GameObject*> objList;

	int width, height;

	GameState gmState = MENU;
};

#endif // !GAME_H