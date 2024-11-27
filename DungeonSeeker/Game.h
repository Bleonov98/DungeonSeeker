#ifndef GAME_H
#define GAME_H

#include <irrklang/irrKlang.h>
using namespace irrklang;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "TextRenderer.h"
#include "ResourceManager.h"
#include "GameObject.h"

#include "Dungeon.h"
#include "Player.h"
#include "Potion.h"
#include "Upgrade.h"

#include <memory>

// to place textures on grid
enum TileType {
	EMPTY,
	MAINTILE,
	MAINTILE_USED,
	TOP,
	RIGHT,
	BOT,
	LEFT,
	CORNER_TOP_LEFT, 
	CORNER_TOP_RIGHT,
	CORNER_BOT_LEFT,
	CORNER_BOT_RIGHT,
	FILLER
};

enum GameState {
	MENU,
	SETTINGS,
	ACTIVE
};

struct Grid {
public:
	Grid(int data) : data(data) {};
	int data;
	glm::vec2 cellPosition, cellSize = glm::vec2(50.0f);
};

struct MapObject {
public:
	MapObject(std::string textureName, glm::vec2 position, glm::vec2 size) : textureName(textureName) {
		mapMat = glm::translate(mapMat, glm::vec3(position, 0.0f));
		mapMat = glm::scale(mapMat, glm::vec3(size, 0.0f));
	};
	std::string textureName;
	GLuint textureID;
	glm::mat4 mapMat;
};

struct Camera {
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float cameraSpeed = 250.0f; // delete after adding the player
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

	void ProcessInventoryKeys();
	void ProcessInput(float dt);
	void Update(float dt);
	void UpdateAnimations(float dt);
	void ProcessCollisions(float dt);
	void ProcessDeaths();

	// level generation
	void GenerateDungeon();
	void GenerateLevel();
	void ClearMap();
	void SetGrid();
	void SetTile();

	// render
	void Render();
	void DrawTexture(Texture texture, glm::vec2 position, glm::vec2 size, glm::vec3 angle = glm::vec3(0.0f), float transparency = 1.0f, glm::vec3 colour = glm::vec3(1.0f));
	void DrawMapObject(std::vector<std::shared_ptr<MapObject>> objects);

	template <typename T>
	void DrawObject(std::vector<std::shared_ptr<T>> objectVector); // vector of objects with instancing
	template <typename T>
	void DrawObject(std::shared_ptr<T> object); // single object without instancing
		// player
	void ShowPlayerStatusBar();
	void ShowPlayerInventory();
	void ShowPlayerStats();
	void DrawDirectionArrow();
	
	void Menu();
	void Settings();

	// game
	std::vector<DropEntry> GetItemsByRarity(ItemRarity rarity);
		// enemies
	void SpawnEnemy();
	void UpdateEnemies(float dt);
	void SpawnExit();

	// utility
	void PlayMusic();

	int GetRandomNumber(int min, int max);
	template<typename T>
	void EraseFromVector(std::vector<std::shared_ptr<T>>& vector);
	void DeleteObjects();

	// pub vars
	bool Keys[1024], KeysProcessed[1024], mouseKeys[8], mouseKeysProcessed[8], close = false;
	float xMouse, yMouse;

	~Game();

private:

	Camera camera;
	glm::mat4 projection, view = glm::mat4(1.0f);

	// sound
	std::unordered_map<std::string, irrklang::ISoundSource*> gameSound;
	std::vector<irrklang::ISoundSource*> music;
	int activeMusic = 0, musicAmount = 0;

	// buttons
	std::vector<std::shared_ptr<TextButton>> menuButtons;
	std::vector<std::shared_ptr<TextButton>> settingButtons;

	// map
	std::vector<std::vector<std::shared_ptr<Grid>>> grid;

	std::vector<std::shared_ptr<MapObject>> mainTileList;
	std::vector<std::shared_ptr<GameObject>> mapObjList;

	// objects
		// general
	std::vector<std::shared_ptr<GameObject>> objList;
	std::vector<std::shared_ptr<DynamicObject>> animObjList;
	std::vector<std::shared_ptr<Character>> characterList;

		// enemies
	std::vector<std::shared_ptr<Enemy>> enemyList;
	std::vector<std::shared_ptr<Vampire>> vampireList;
	
		// projectiles
	std::vector<std::shared_ptr<MagicSphere>> projectileList;

		// items
	std::vector<std::shared_ptr<Item>> itemList;

		// - - - some type
	bool statusShow = false, exitSpawned = false;
	int width, height;
	GameState gmState = MENU;
};

#endif // !GAME_H