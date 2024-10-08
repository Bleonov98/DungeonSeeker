#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "TextRenderer.h"
#include "ResourceManager.h"
#include "GameObject.h"

#include "Grid.h"
#include "Dungeon.h"

#include <memory>

enum GameState {
	MENU,
	SETTINGS,
	ACTIVE,
	PAUSED,
	TEST
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
	void InitTextButtons();
	void GenerateLevel();

	void ProcessInput(float dt);
	void Update(float dt);

	void Render();
	void DrawTexture(Texture texture, glm::vec2 position, glm::vec2 size);
	template <typename T>
	void DrawObject(std::vector<T*> objectVector);
	void DrawGrid();
	//void DrawStats();

	void Menu();
	void Settings();
	//void Restart();

	// game

	// pub vars
	bool Keys[1024], KeysProcessed[1024], mouseKeys[8], mouseKeysProcessed[8], close = false;
	float xMouse, yMouse;

	~Game();

private:

	glm::mat4 projection;

	// buttons
	std::vector<std::shared_ptr<TextButton>> menuButtons;
	std::vector<std::shared_ptr<TextButton>> settingButtons;

	// objects
	std::vector<std::shared_ptr<GameObject>> objList;

		// - - - some type

	int width, height;

	GameState gmState = TEST;
};

#endif // !GAME_H