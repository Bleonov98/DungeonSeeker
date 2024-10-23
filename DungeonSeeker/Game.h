#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "TextRenderer.h"
#include "ResourceManager.h"
#include "GameObject.h"

#include "Dungeon.h"

#include <memory>

// to place textures on grid
enum TileType {
	EMPTY,
	MAINTILE,
	TOP,
	RIGHT,
	BOT,
	LEFT,
	CORNER_TOP_LEFT, 
	CORNER_TOP_RIGHT,
	CORNER_BOT_LEFT,
	CORNER_BOT_RIGHT
};

enum GameState {
	MENU,
	SETTINGS,
	ACTIVE,
	PAUSED
};

struct Grid {
public:
	Grid(int data) : data(data) {};
	int data;
	glm::vec2 cellPosition, cellSize = glm::vec2(20.0f);
};

struct MapObject {
public:
	MapObject(std::string textureName, glm::vec2 position, glm::vec2 size) : textureName(textureName) {
		mapMat = glm::translate(mapMat, glm::vec3(position, 0.0f));
		mapMat = glm::scale(mapMat, glm::vec3(size, 0.0f));
	};
	std::string textureName;
	glm::mat4 mapMat;
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

	void ProcessInput(float dt);
	void Update(float dt);

	// level generation
	void GenerateLevel();
	void SetGrid();
	void SetTile();

	// render
	void Render();
	void DrawTexture(Texture texture, glm::vec2 position, glm::vec2 size);
	void DrawMapObject(std::vector<MapObject> objects);
	template <typename T>
	void DrawObject(std::vector<T*> objectVector);
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

	// map
	std::vector<std::vector<std::shared_ptr<Grid>>> grid;
	std::vector<std::shared_ptr<MapObject>> mainTileList;

	// objects
	std::vector<std::shared_ptr<GameObject>> objList;

		// - - - some type

	int width, height;

	GameState gmState = MENU;
};

#endif // !GAME_H