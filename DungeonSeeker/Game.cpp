#include "Game.h"

#include <irrklang/irrKlang.h>
using namespace irrklang;

std::unique_ptr<TextRenderer> text;
ISoundEngine* sound = irrklang::createIrrKlangDevice();

std::unique_ptr<Renderer> renderer;
std::unique_ptr<Dungeon> dungeon;
std::shared_ptr<Player> player;

void Game::Init()
{
    srand(time(NULL));
    LoadResources();

    projection = glm::ortho(0.0f, static_cast<float>(this->width), static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
    text = std::make_unique<TextRenderer>(this->width, this->height);
    text->Load("../fonts/Garamond.ttf", 24);

    InitObjects();
    InitTextButtons();
}

void Game::LoadResources()
{
    ResourceManager::LoadShader("../shaders/vShader.vx", "../shaders/fShader.ft", "spriteShader");
    ResourceManager::LoadShader("../shaders/menuVShader.vx", "../shaders/menuFShader.ft", "menuShader");

    // textures
    ResourceManager::LoadTexture("../textures/main/menu.png", true, "menuTexture");
    ResourceManager::LoadTexture("../textures/main/cursor.png", true, "cursorTexture");

    // map
    ResourceManager::LoadTexture("../textures/map/main_tile.png", true, "mainTile");
    ResourceManager::LoadTexture("../textures/map/filler_tile.png", true, "fillerTile");
    ResourceManager::LoadTexture("../textures/map/main_tile_crack.png", true, "mainTileCrack");
    ResourceManager::LoadTexture("../textures/map/main_tile_stone_0.png", true, "mainTileStone0");
    ResourceManager::LoadTexture("../textures/map/main_tile_stone_1.png", true, "mainTileStone1");
    ResourceManager::LoadTexture("../textures/map/main_tile_stone_2.png", true, "mainTileStone2");

    ResourceManager::LoadTexture("../textures/map/top_tile.png", true, "topTile");
    ResourceManager::LoadTexture("../textures/map/bot_tile.png", true, "botTile");
    ResourceManager::LoadTexture("../textures/map/left_tile.png", true, "leftTile");
    ResourceManager::LoadTexture("../textures/map/right_tile.png", true, "rightTile");

    ResourceManager::LoadTexture("../textures/map/left_top_corner_tile.png", true, "leftTopTile");
    ResourceManager::LoadTexture("../textures/map/right_top_corner_tile.png", true, "rightTopTile");
    ResourceManager::LoadTexture("../textures/map/left_bot_corner_tile.png", true, "leftBotTile");
    ResourceManager::LoadTexture("../textures/map/right_bot_corner_tile.png", true, "rightBotTile");

    // characters
        // player
    for (size_t i = 0; i < 4; i++)
    {
        ResourceManager::LoadTexture(("../textures/player/player_" + std::to_string(i) + ".png").c_str(), true, "player" + std::to_string(i));
    }
    for (size_t i = 0; i < 3; i++)
    {
        ResourceManager::LoadTexture(("../textures/player/player_attack_" + std::to_string(i) + ".png").c_str(), true, "playerAttack" + std::to_string(i));
    }
        // skeleton
    for (size_t i = 0; i < 4; i++)
    {
        ResourceManager::LoadTexture(("../textures/enemies/skeleton/skeleton_" + std::to_string(i) + ".png").c_str(), true, "skeleton" + std::to_string(i));
    }      
        // skull
    for (size_t i = 0; i < 4; i++)
    {
        ResourceManager::LoadTexture(("../textures/enemies/skull/skull_" + std::to_string(i) + ".png").c_str(), true, "skull" + std::to_string(i));
    }
        // vampire
    for (size_t i = 0; i < 4; i++)
    {
        ResourceManager::LoadTexture(("../textures/enemies/vamp/vamp_" + std::to_string(i) + ".png").c_str(), true, "vamp" + std::to_string(i));
    }
}

void Game::InitObjects()
{
    renderer = std::make_unique<Renderer>();

    Grid cell(0); // for convenience

    // PLAYER INITIALIZATION
    player = std::make_shared<Player>(glm::vec2(0, 0), cell.cellSize * 1.75f, 175.0f);
        // walking-stand textures + animation
    for (size_t i = 0; i < 4; i++)
    {
        player->SetTexture("player" + std::to_string(i));
    }
    player->AddAnimation("main", 0, 4, 0.4f, true);
        // attack textures + animations
    for (size_t i = 0; i < 3; i++)
    {
        player->SetTexture("playerAttack" + std::to_string(i));
    }
    player->AddAnimation("attack", 4, 3, 0.15f);

    objList.push_back(player);
    animObjList.push_back(player);
    // - - - - - - - - - - - - - - - - - -

    GenerateDungeon();
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

// level generation
void Game::GenerateDungeon()
{
    dungeon = std::make_unique<Dungeon>();
    GenerateLevel();
    Grid cell(0); // for convenience

    // find nearest room to start
    float minValue = FLT_MAX;
    glm::vec2 playerPos;
    for (size_t i = 0; i < dungeon->rooms.size(); i++)
    {
        float distance = glm::distance(dungeon->rooms[i].position, glm::vec2(0, 0));
        if (distance < minValue) {
            minValue = distance;
            playerPos = dungeon->rooms[i].position * cell.cellSize;
        }
    }
    player->SetPos(playerPos);
}

void Game::GenerateLevel()
{
    dungeon->GenerateDungeon(width / 10.0f, height / 10.0f);
    SetGrid();
    SetTile();

    // enemies
    int eCnt = 6 + rand() % 4;
    for (size_t i = 0; i < eCnt; i++)
    {
        SpawnEnemy();
    }
}

void Game::SetGrid()
{
    grid.resize(dungeon->GetHeight(), std::vector<std::shared_ptr<Grid>>(dungeon->GetWidth()));

    for (size_t i = 0; i < grid.size(); i++)
    {
        for (size_t j = 0; j < grid[i].size(); j++)
        {
            std::shared_ptr<Grid> cell = std::make_shared<Grid>(EMPTY);
            cell->cellPosition = glm::vec2(cell->cellSize.x * j, cell->cellSize.y * i);
            grid[i][j] = cell;
        }
    }

    for (size_t i = 0; i < dungeon->rooms.size(); i++)
    {
        for (size_t j = 0; j < dungeon->rooms[i].height; j++)
        {
            for (size_t k = 0; k < dungeon->rooms[i].width; k++)
            {
                grid[std::floor(dungeon->rooms[i].position.y) + j][std::floor(dungeon->rooms[i].position.x) + k]->data = MAINTILE;
            }
        }
    }

    for (size_t i = 0; i < dungeon->corridors.size(); i++)
    {
        for (int j = 0; j < dungeon->corridors[i].width; j++)
        {
            for (int k = 0; k < dungeon->corridors[i].length; k++)
            {
                grid[std::floor(dungeon->corridors[i].position.y) + j][std::floor(dungeon->corridors[i].position.x) + k]->data = MAINTILE;
            }
        }
    }

    for (size_t i = 1; i < grid.size() - 1; i++)
    {
        for (size_t j = 1; j < grid[i].size() - 1; j++)
        {
            // define tiletype
            // TOP SIDE
            if (i == 1 && grid[i - 1][j]->data == MAINTILE) grid[i - 1][j]->data = TOP; // here mb smth else - to think !!!!!!!!!!!!!

            if (grid[i][j]->data == MAINTILE && grid[i - 1][j]->data == EMPTY) 
            {
                grid[i - 1][j]->data = TOP;

                if (grid[i][j - 1]->data == EMPTY) {
                    grid[i - 1][j - 1]->data = CORNER_TOP_LEFT;
                    grid[i][j - 1]->data = LEFT;
                }
                else if (grid[i][j + 1]->data == EMPTY) {
                    grid[i - 1][j + 1]->data = CORNER_TOP_RIGHT;
                    grid[i][j + 1]->data = RIGHT;
                }
            }
            // BOT SIDE
            else if (grid[i][j]->data == MAINTILE && grid[i + 1][j]->data == EMPTY) 
            {
                grid[i + 1][j]->data = BOT;

                if (grid[i][j - 1]->data == EMPTY) {
                    grid[i + 1][j - 1]->data = CORNER_BOT_LEFT;
                    grid[i][j - 1]->data = LEFT;
                }
                else if (grid[i][j + 1]->data == EMPTY) {
                    grid[i + 1][j + 1]->data = CORNER_BOT_RIGHT;
                    grid[i][j + 1]->data = RIGHT;
                }
            }
            // RIGHT SIDE
            else if (grid[i][j]->data == MAINTILE && grid[i][j + 1]->data == EMPTY)
                grid[i][j + 1]->data = RIGHT;
            // LEFT SIDE
            else if (grid[i][j]->data == MAINTILE && grid[i][j - 1]->data == EMPTY)
                grid[i][j - 1]->data = LEFT;
        }
    }

    for (size_t i = 1; i < grid.size() - 1; i++)
    {
        for (size_t j = 1; j < grid[i].size() - 1; j++)
        {
            // corrections
            if (grid[i][j]->data == EMPTY) {
                if (grid[i - 1][j]->data != EMPTY && grid[i][j + 1]->data != EMPTY) {
                    grid[i - 1][j + 1]->data = FILLER;
                }
                else if (grid[i - 1][j]->data != EMPTY && grid[i][j - 1]->data != EMPTY) {
                    grid[i - 1][j - 1]->data = FILLER;
                }
                else if (grid[i + 1][j]->data != EMPTY && grid[i][j + 1]->data != EMPTY) {
                    grid[i + 1][j + 1]->data = FILLER;
                }
                else if (grid[i + 1][j]->data != EMPTY && grid[i][j - 1]->data != EMPTY) {
                    grid[i + 1][j - 1]->data = FILLER;
                }
            }
        }
    }
}

void Game::SetTile()
{
    std::vector<std::string> nameList;
    for (auto i : ResourceManager::textures)
    {
        if (i.first.find("mainTile") != std::string::npos) nameList.push_back(i.first);
    }

    for (size_t i = 0; i < grid.size(); i++)
    {
        for (size_t j = 0; j < grid[i].size(); j++)
        {
            switch (grid[i][j]->data)
            {
            case MAINTILE: 
            {
                std::shared_ptr<MapObject> tile = std::make_shared<MapObject>(nameList[GetRandomNumber(0, nameList.size())], grid[i][j]->cellPosition, grid[i][j]->cellSize);
                tile->textureID = ResourceManager::GetTexture(tile->textureName).GetID();
                mainTileList.push_back(tile);
            }
            break;

            case TOP: 
            {
                std::shared_ptr<GameObject> mapObj = std::make_shared<GameObject>(grid[i][j]->cellPosition, grid[i][j]->cellSize);
                mapObj->SetTexture("topTile");
                mapObjList.push_back(mapObj);
            }
            break;

            case BOT:
            {
                std::shared_ptr<GameObject> mapObj = std::make_shared<GameObject>(grid[i][j]->cellPosition, grid[i][j]->cellSize);
                mapObj->SetTexture("botTile");
                mapObjList.push_back(mapObj);
            }
            break;

            case LEFT:
            {
                std::shared_ptr<GameObject> mapObj = std::make_shared<GameObject>(grid[i][j]->cellPosition, grid[i][j]->cellSize);
                mapObj->SetTexture("leftTile");
                mapObjList.push_back(mapObj);
            }
            break;

            case RIGHT:
            {
                std::shared_ptr<GameObject> mapObj = std::make_shared<GameObject>(grid[i][j]->cellPosition, grid[i][j]->cellSize);
                mapObj->SetTexture("rightTile");
                mapObjList.push_back(mapObj);
            }
            break;

            case CORNER_TOP_LEFT:
            {
                std::shared_ptr<GameObject> mapObj = std::make_shared<GameObject>(grid[i][j]->cellPosition, grid[i][j]->cellSize);
                mapObj->SetTexture("leftTopTile");
                mapObjList.push_back(mapObj);
            }
            break;

            case CORNER_TOP_RIGHT:
            {
                std::shared_ptr<GameObject> mapObj = std::make_shared<GameObject>(grid[i][j]->cellPosition, grid[i][j]->cellSize);
                mapObj->SetTexture("rightTopTile");
                mapObjList.push_back(mapObj);
            }
            break;

            case CORNER_BOT_LEFT:
            {
                std::shared_ptr<GameObject> mapObj = std::make_shared<GameObject>(grid[i][j]->cellPosition, grid[i][j]->cellSize);
                mapObj->SetTexture("leftBotTile");
                mapObjList.push_back(mapObj);
            }
            break;

            case CORNER_BOT_RIGHT:
            {
                std::shared_ptr<GameObject> mapObj = std::make_shared<GameObject>(grid[i][j]->cellPosition, grid[i][j]->cellSize);
                mapObj->SetTexture("rightBotTile");
                mapObjList.push_back(mapObj);
            }
            break;
            case FILLER:
            {
                std::shared_ptr<GameObject> mapObj = std::make_shared<GameObject>(grid[i][j]->cellPosition, grid[i][j]->cellSize);
                mapObj->SetTexture("fillerTile");
                mapObjList.push_back(mapObj);
            }
            break;
            default:
                break;
            }
        }
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
        // Move screen
        float midScreenX = player->GetPos().x - this->width / 2.0f + player->GetSize().x / 2.0f;
        float midScreenY = player->GetPos().y - this->height / 2.0f + player->GetSize().y / 2.0f;
        camera.cameraPos = glm::vec3(midScreenX, midScreenY, 1.0f);
        if (camera.cameraPos.x < 0.0f) camera.cameraPos.x = 0.0f;
        if (camera.cameraPos.y < 0.0f) camera.cameraPos.y = 0.0f;
 
        if (Keys[GLFW_KEY_W]) 
            player->Move(DIR_UP, dt);
        else if (Keys[GLFW_KEY_S]) 
            player->Move(DIR_DOWN, dt);

        if (Keys[GLFW_KEY_A]) 
            player->Move(DIR_LEFT, dt);
        else if (Keys[GLFW_KEY_D]) 
            player->Move(DIR_RIGHT, dt);

        if (mouseKeys[GLFW_MOUSE_BUTTON_LEFT] && !mouseKeysProcessed[GLFW_MOUSE_BUTTON_LEFT]) {
            player->Attack();
            mouseKeysProcessed[GLFW_MOUSE_BUTTON_LEFT] = true;
        }
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
        UpdateAnimations(dt);

        player->ProcessAction(dt);

        // update borders after position changes
        for (auto i : objList)
        {
            i->UpdateAABB();
        }

        // interactions
        ProcessCollisions(dt);
    }
}

void Game::UpdateAnimations(float dt)
{ 
    for (size_t i = 0; i < animObjList.size(); i++)
    {
        animObjList[i]->PlayAnimation(dt);
    }

}

void Game::ProcessCollisions(float dt)
{
    // player collisions
        // map collision
    for (size_t i = 0; i < mapObjList.size(); i++)
    {
        if (i == 0) player->ProcessCollision((*mapObjList[i]), true, dt); // bool prevents double collision
        else player->ProcessCollision((*mapObjList[i]), false, dt);
        
    }
        // attack collision
    if (player->GetAction() == ATTACK) {
        for (size_t i = 0; i < enemyList.size(); i++)
        {
            player->AttackCollision(enemyList[i]);
        }
    }

}

// utility
int Game::GetRandomNumber(int min, int max)
{
    int pseudoRandNum = 0;
    static int number = 0;

    if (number > 14 + rand() % 10) {
        number = 0;
        pseudoRandNum = min + rand() % (max - min);
    }
    else {
        pseudoRandNum = min;
    }

    number++;
    return pseudoRandNum;
}

void Game::SpawnEnemy()
{
    enum EnemyType {
        SKELETON = 0,
        SKULL,
        VAMP
    };
    EnemyType type = static_cast<EnemyType>(rand() % (VAMP + 1));

    // random place
    glm::vec2 enemyPos;
    while (true)
    {
        int row = rand() % (grid.size() - 2), col = rand() % (grid[0].size() - 2);
        if (grid[row][col]->data == MAINTILE && grid[row + 2][col]->data == MAINTILE && grid[row][col + 2]->data == MAINTILE) {
            enemyPos = grid[row][col]->cellPosition + grid[row][col]->cellSize / 2.0f;
            grid[row][col]->data = MAINTILE_USED;
            break;
        }
    }

    std::shared_ptr<Enemy> enemy;
    if (type == SKELETON) 
    {
        std::shared_ptr<Skeleton> skeleton = std::make_shared<Skeleton>(enemyPos, player->GetSize(), 75.0f);
        for (size_t i = 0; i < 4; i++)
        {
            skeleton->SetTexture("skeleton" + std::to_string(i));
        }
        skeleton->AddAnimation("main", 0, 4, 0.4f, true);
        enemy = skeleton;
    }
    else if (type == SKULL)
    {
        std::shared_ptr<Skull> skull = std::make_shared<Skull>(enemyPos, player->GetSize(), 75.0f);
        for (size_t i = 0; i < 4; i++)
        {
            skull->SetTexture("skull" + std::to_string(i));
        }
        skull->AddAnimation("main", 0, 4, 0.4f, true);
        enemy = skull;
    }
    else
    {
        std::shared_ptr<Vampire> vamp = std::make_shared<Vampire>(enemyPos, player->GetSize(), 75.0f);
        for (size_t i = 0; i < 4; i++)
        {
            vamp->SetTexture("vamp" + std::to_string(i));
        }
        vamp->AddAnimation("main", 0, 4, 0.4f, true);
        enemy = vamp;
    }

    objList.push_back(enemy);
    animObjList.push_back(enemy);
    enemyList.push_back(enemy);
}

// render
void Game::Render()
{
    view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);

    // background/map/stats
    DrawMapObject(mainTileList);
    DrawObject(mapObjList);

    // chars
    DrawObject(enemyList);
    DrawObject(player);

#ifdef _TESTING
    dungeon->DrawDungeon();
#endif

    if (gmState == MENU) Menu();
    else if (gmState == SETTINGS) Settings();

    if (gmState != ACTIVE) DrawTexture(ResourceManager::GetTexture("cursorTexture"), glm::vec2(xMouse, yMouse), glm::vec2(30.0f, 32.0f));
}

void Game::DrawTexture(Texture texture, glm::vec2 position, glm::vec2 size) // menu texture
{
    ResourceManager::GetShader("menuShader").Use();
    ResourceManager::GetShader("menuShader").SetMatrix4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(size, 0.0f));

    ResourceManager::GetShader("menuShader").SetMatrix4("model", model);
    renderer->DrawTexture(texture);
} 

void Game::DrawMapObject(std::vector<std::shared_ptr<MapObject>> objects)
{
    ResourceManager::GetShader("spriteShader").Use();
    ResourceManager::GetShader("spriteShader").SetMatrix4("projection", projection);
    ResourceManager::GetShader("spriteShader").SetMatrix4("view", view);

    std::vector<glm::mat4> instMat;
    std::vector<glm::vec3> instCol(objects.size(), glm::vec3(1.0f));
    std::vector<GLuint> textureIDs;

    for (auto i : objects)
    {
        instMat.push_back(i->mapMat);
        textureIDs.push_back(i->textureID);
    }

    ResourceManager::GetShader("spriteShader").SetBool("instanced", true);
    ResourceManager::GetShader("spriteShader").SetBool("menu", gmState == MENU);

#ifdef _TESTING
    ResourceManager::GetShader("spriteShader").SetBool("test", false);
#endif

    renderer->Draw(instMat, instCol, textureIDs);
}

template <typename T>
void Game::DrawObject(std::vector<std::shared_ptr<T>> objectVector)
{
    ResourceManager::GetShader("spriteShader").Use();
    ResourceManager::GetShader("spriteShader").SetMatrix4("projection", projection);
    ResourceManager::GetShader("spriteShader").SetMatrix4("view", view);

    std::vector<glm::mat4> instMat;
    std::vector<glm::vec3> instCol;
    std::vector<GLuint> textureIDs;

    for (auto i : objectVector)
    {
        i->RefreshMatrix();
        instMat.push_back(i->GetMatrix());
        instCol.push_back(i->GetColour());
        textureIDs.push_back(ResourceManager::GetTexture(i->GetTextureName()).GetID());
    }

    ResourceManager::GetShader("spriteShader").SetBool("instanced", true);

    if (gmState != ACTIVE) ResourceManager::GetShader("spriteShader").SetBool("menu", true);
    else ResourceManager::GetShader("spriteShader").SetBool("menu", false);

    renderer->Draw(instMat, instCol, textureIDs);
}

template <typename T>
void Game::DrawObject(std::shared_ptr<T> object)
{
    ResourceManager::GetShader("spriteShader").Use();
    ResourceManager::GetShader("spriteShader").SetMatrix4("projection", projection);
    ResourceManager::GetShader("spriteShader").SetMatrix4("view", view);

    //
    object->RefreshMatrix();
    ResourceManager::GetShader("spriteShader").SetMatrix4("uModel", object->GetMatrix());
    ResourceManager::GetShader("spriteShader").SetVector3f("uColour", object->GetColour());

    //
    ResourceManager::GetShader("spriteShader").SetBool("instanced", false);
    if (gmState != ACTIVE) ResourceManager::GetShader("spriteShader").SetBool("menu", true);
    else ResourceManager::GetShader("spriteShader").SetBool("menu", false);
    
    renderer->DrawTexture(ResourceManager::GetTexture(object->GetTextureName()));
}

Game::~Game()
{
    delete sound;

    objList.clear();
    // -----
    menuButtons.clear();
    settingButtons.clear();
}
