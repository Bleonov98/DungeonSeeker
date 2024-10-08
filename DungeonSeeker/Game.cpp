#include "Game.h"

#include <irrklang/irrKlang.h>
using namespace irrklang;

std::unique_ptr<TextRenderer> text;
ISoundEngine* sound = irrklang::createIrrKlangDevice();

std::unique_ptr<Renderer> renderer;
std::unique_ptr<Dungeon> dungeon;

void Game::Init()
{
    srand(time(NULL));
    LoadResources();

    projection = glm::ortho(0.0f, static_cast<float>(this->width), static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
    text = std::make_unique<TextRenderer>(this->width, this->height);
    text->Load("../fonts/Garamond.ttf", 24);

    renderer = std::make_unique<Renderer>();

    dungeon = std::make_unique<Dungeon>();

    InitObjects();
    InitTextButtons();
}

void Game::LoadResources()
{
    ResourceManager::LoadShader("../shaders/vShader.vx", "../shaders/fShader.ft", "spriteShader");

    // textures
    ResourceManager::LoadTexture("../textures/main/menu.png", true, "menuTexture");
    ResourceManager::LoadTexture("../textures/main/cursor.png", true, "cursorTexture");
}

void Game::InitObjects()
{
    GenerateLevel();
}

void Game::InitTextButtons()
{
    // menu
    std::shared_ptr<TextButton> start = std::make_shared<TextButton>("Start", glm::vec2(this->width / 2.0f - 25.0f, this->height / 2.0f - 20.0f), this->width, this->height);
    start->Load("../fonts/Garamond.ttf", 24);
    start->SetFunction([&]() { this->gmState = ACTIVE; });
    menuButtons.push_back(start);

    std::shared_ptr<TextButton> settings = std::make_shared<TextButton>("Settings", glm::vec2(this->width / 2.0f - 40.0f, this->height / 2.0f + 10.0f), this->width, this->height);
    settings->Load("../fonts/Garamond.ttf", 24);
    settings->SetFunction([&]() { this->gmState = SETTINGS; });
    menuButtons.push_back(settings);

    std::shared_ptr<TextButton> exit = std::make_shared<TextButton>("Exit", glm::vec2(this->width / 2.0f - 22.0f, this->height / 2.0f + 40.0f), this->width, this->height);
    exit->Load("../fonts/Garamond.ttf", 24);
    exit->SetFunction([&]() { this->close = true; });
    menuButtons.push_back(exit);

    // settings
    std::shared_ptr<TextButton> back = std::make_shared<TextButton>("Back", glm::vec2(this->width / 2.0f - 22.0f, this->height / 2.0f + 40.0f), this->width, this->height);
    back->Load("../fonts/Garamond.ttf", 24);
    back->SetFunction([&]() { this->gmState = MENU; });
    settingButtons.push_back(back);
}

void Game::GenerateLevel()
{

}

void Game::Menu()
{
    DrawTexture(ResourceManager::GetTexture("menuTexture"), glm::vec2(650.0f, 275.0f), glm::vec2(300.0f));

    text->RenderText("MENU", glm::vec2(this->width / 2.0f - 65.0f, this->height / 2.0f - 120.0f), 1.75f);

    for (size_t i = 0; i < menuButtons.size(); i++)
    {
        menuButtons[i]->RenderButton(1.0f);
    }
}

void Game::Settings()
{
    DrawTexture(ResourceManager::GetTexture("menuTexture"), glm::vec2(650.0f, 275.0f), glm::vec2(300.0f));

    text->RenderText("SETTINGS", glm::vec2(this->width / 2.0f - 95.0f, this->height / 2.0f - 120.0f), 1.75f);

    for (auto i : settingButtons)
    {
        i->RenderButton(1.0f);
    }
}

// main
void Game::ProcessInput(float dt)
{
    if (this->Keys[GLFW_KEY_ESCAPE] && !this->KeysProcessed[GLFW_KEY_ESCAPE] && (gmState == MENU || gmState == ACTIVE)) {
       
        gmState == ACTIVE ? gmState = MENU : gmState = ACTIVE;

        this->KeysProcessed[GLFW_KEY_ESCAPE] = true;
    }

    if (gmState == ACTIVE) {
        
    }
    else if (gmState == MENU) {
        for (auto i : menuButtons)
        {
            if (i->TextCollision(xMouse, yMouse)) {
                i->SetTextColour(glm::vec3(0.9f));
                if (mouseKeys[GLFW_MOUSE_BUTTON_LEFT] && !mouseKeysProcessed[GLFW_MOUSE_BUTTON_LEFT]) {
                    i->CallFunction();
                    mouseKeysProcessed[GLFW_MOUSE_BUTTON_LEFT] = true;
                }
            }
            else i->SetTextColour(glm::vec3(0.4f));
        }
    }
    else if (gmState == SETTINGS) {
        for (auto i : settingButtons)
        {
            if (i->TextCollision(xMouse, yMouse)) {
                i->SetTextColour(glm::vec3(0.9f));
                if (mouseKeys[GLFW_MOUSE_BUTTON_LEFT] && !mouseKeysProcessed[GLFW_MOUSE_BUTTON_LEFT]) {
                    i->CallFunction();
                    mouseKeysProcessed[GLFW_MOUSE_BUTTON_LEFT] = true;
                }
            }
            else i->SetTextColour(glm::vec3(0.4f));
        }
    }
}

void Game::Update(float dt)
{
    if (gmState == ACTIVE) {

        // actions

        // update borders after position changes
        for (auto i : objList)
        {
            i->UpdateAABB();
        }

        // interactions
    }
}

// render
void Game::Render()
{
    // background/map/stats

    if (gmState == MENU) Menu();
    else if (gmState == SETTINGS) Settings();

#ifdef _TESTING
    dungeon->DrawDungeon();
#endif

    DrawTexture(ResourceManager::GetTexture("cursorTexture"), glm::vec2(xMouse, yMouse), glm::vec2(30.0f, 32.0f));
}

void Game::DrawTexture(Texture texture, glm::vec2 position, glm::vec2 size)
{
    ResourceManager::GetShader("spriteShader").Use();
    ResourceManager::GetShader("spriteShader").SetMatrix4("projection", projection);
    ResourceManager::GetShader("spriteShader").SetVector3f("uColour", glm::vec3(1.0f));

    ResourceManager::GetShader("spriteShader").SetBool("menu", false);
    ResourceManager::GetShader("spriteShader").SetBool("instanced", false);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(size, 0.0f));

    ResourceManager::GetShader("spriteShader").SetMatrix4("uModel", model);

#ifdef _TESTING
    ResourceManager::GetShader("spriteShader").SetBool("test", false);
#endif

    renderer->DrawTexture(texture);
}

void Game::DrawGrid()
{

}

template <typename T>
void Game::DrawObject(std::vector<T*> objectVector)
{
    ResourceManager::GetShader("spriteShader").Use();
    ResourceManager::GetShader("spriteShader").SetMatrix4("projection", projection);

    std::vector<glm::mat4> instMat;
    std::vector<glm::vec3> instCol;

    for (auto i : objectVector)
    {
        i->RefreshMatrix();
        instMat.push_back(i->GetMatrix());
        instCol.push_back(i->GetColour());
    }

    ResourceManager::GetShader("spriteShader").SetBool("instanced", true);

    if (gmState != ACTIVE) ResourceManager::GetShader("spriteShader").SetBool("menu", true);
    else ResourceManager::GetShader("spriteShader").SetBool("menu", false);

    renderer->Draw(ResourceManager::GetTexture(objectVector[0].GetTextureName()), instMat, instCol);
}

Game::~Game()
{
    delete sound;

    objList.clear();
    // -----
    menuButtons.clear();
    settingButtons.clear();
}
