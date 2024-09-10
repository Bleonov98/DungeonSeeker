#include "Game.h"

#include <irrklang/irrKlang.h>
using namespace irrklang;

std::unique_ptr<TextRenderer> text;
ISoundEngine* sound = irrklang::createIrrKlangDevice();

Renderer* renderer;

void Game::Init()
{
    LoadResources();

    projection = glm::ortho(0.0f, static_cast<float>(this->width), static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
    text = std::make_unique<TextRenderer>(this->width, this->height);
    text->Load("../fonts/Garamond.ttf", 24);

    renderer = new Renderer();

    InitObjects();
    InitTextButtons();
}

void Game::LoadResources()
{
    ResourceManager::LoadShader("../shaders/vShader.vx", "../shaders/fShader.ft", "mainShader");
}

void Game::InitObjects()
{

}

void Game::InitTextButtons()
{
    std::shared_ptr<TextButton> start = std::make_shared<TextButton>("Start", glm::vec2(this->width / 2.0f - 25.0f, this->height / 2.0f - 20.0f), this->width, this->height);
    start->Load("../fonts/Garamond.ttf", 24);
    start->SetFunction([&]() {
        this->gmState = ACTIVE;
    });
    textButtons.push_back(start);

    std::shared_ptr<TextButton> settings = std::make_shared<TextButton>("Settings", glm::vec2(this->width / 2.0f - 40.0f, this->height / 2.0f + 10.0f), this->width, this->height);
    settings->Load("../fonts/Garamond.ttf", 24);
    settings->SetFunction([&]() { this->gmState = SETTINGS; });
    textButtons.push_back(settings);

    std::shared_ptr<TextButton> exit = std::make_shared<TextButton>("Exit", glm::vec2(this->width / 2.0f - 22.0f, this->height / 2.0f + 40.0f), this->width, this->height);
    exit->Load("../fonts/Garamond.ttf", 24);
    exit->SetFunction([&]() { this->close = true; });
    textButtons.push_back(exit);
}

void Game::Menu()
{
    text->RenderText("MENU", glm::vec2(this->width / 2.0f - 65.0f, this->height / 2.0f - 100.0f), 1.75f);

    for (size_t i = 0; i < textButtons.size(); i++)
    {
        textButtons[i]->RenderButton(1.0f);
    }
}

void Game::ProcessInput(float dt)
{
    if (this->Keys[GLFW_KEY_ESCAPE] && !this->KeysProcessed[GLFW_KEY_ESCAPE] && (gmState == MENU || gmState == ACTIVE)) {
       
        gmState == ACTIVE ? gmState = MENU : gmState = ACTIVE;

        this->KeysProcessed[GLFW_KEY_ESCAPE] = true;
    }

    if (gmState == ACTIVE) {
        
    }
    else if (gmState == MENU) {
        for (auto i : textButtons)
        {
            if (i->TextCollision(xMouse, yMouse)) {
                i->SetTextColour(glm::vec3(0.9f));
                if (mouseKeys[GLFW_MOUSE_BUTTON_LEFT] && !mouseKeysProcessed[GLFW_MOUSE_BUTTON_LEFT]) {
                    i->CallFunction();
                    mouseKeys[GLFW_MOUSE_BUTTON_LEFT] = true;
                }
            }
            else i->SetTextColour(glm::vec3(0.0f));
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

void Game::Render()
{
    // background/map/stats

    if (gmState == MENU) Menu();
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

    if (gmState != ACTIVE) ResourceManager::GetShader("spriteShader").SetBool("menu", true);
    else ResourceManager::GetShader("spriteShader").SetBool("menu", false);

    renderer->Draw(ResourceManager::GetTexture(objectVector[0].GetTextureName()), instMat, instCol);
}

Game::~Game()
{
    text.release();
    delete sound;

    objList.clear();
    // -----
}
