#include "Game.h"

#include <irrklang/irrKlang.h>
using namespace irrklang;

std::unique_ptr<TextRenderer> text, pixelText;
ISoundEngine* sound = irrklang::createIrrKlangDevice();

std::unique_ptr<Renderer> renderer;
std::unique_ptr<Dungeon> dungeon;
std::unique_ptr<GameObject> exit;

std::shared_ptr<Player> player;

void Game::Init()
{
    srand(time(NULL));
    LoadResources();

    projection = glm::ortho(0.0f, static_cast<float>(this->width), static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
    text = std::make_unique<TextRenderer>(this->width, this->height);
    text->Load("../fonts/Garamond.ttf", 24);

    pixelText = std::make_unique<TextRenderer>(this->width, this->height);
    pixelText->Load("../fonts/Pixel.ttf", 24);

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

    // status bar
    ResourceManager::LoadTexture("../textures/main/healthBar.png", true, "healthBarTexture");
    ResourceManager::LoadTexture("../textures/main/healthBarWrap.png", true, "healthBarWrapTexture");
    ResourceManager::LoadTexture("../textures/main/expBar.png", true, "expBarTexture");
    ResourceManager::LoadTexture("../textures/main/expBarWrap.png", true, "expBarWrapTexture");
    ResourceManager::LoadTexture("../textures/main/inventory.png", true, "inventoryTexture");
    ResourceManager::LoadTexture("../textures/main/exit.png", true, "exitTexture");

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

    // Items, Projectiles
    for (size_t i = 0; i < 5; i++)
    {
        ResourceManager::LoadTexture(("../textures/enemies/projectile/fireball_" + std::to_string(i) + ".png").c_str(), true, "fireball" + std::to_string(i));
    }

    for (size_t i = 0; i < 4; i++)
    {
        ResourceManager::LoadTexture(("../textures/items/potions/hpPotion_small_" + std::to_string(i) + ".png").c_str(), true, "hpPotionSmall" + std::to_string(i));
        ResourceManager::LoadTexture(("../textures/items/potions/hpPotion_" + std::to_string(i) + ".png").c_str(), true, "hpPotion" + std::to_string(i));
    }

    for (size_t i = 0; i < 4; i++)
    {
        ResourceManager::LoadTexture(("../textures/items/potions/msPotion_small_" + std::to_string(i) + ".png").c_str(), true, "msPotionSmall" + std::to_string(i));
        ResourceManager::LoadTexture(("../textures/items/potions/msPotion_" + std::to_string(i) + ".png").c_str(), true, "msPotion" + std::to_string(i));
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
    characterList.push_back(player);
    // - - - - - - - - - - - - - - - - - -

    GenerateDungeon();
    exit = std::make_unique<GameObject>(glm::vec2(9999.9f, 9999.9f), glm::vec2(50.0f));
    exit->SetTexture("exitTexture");
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
void Game::ProcessInventoryKeys()
{
    if (KeysProcessed[GLFW_KEY_1] || KeysProcessed[GLFW_KEY_2] || KeysProcessed[GLFW_KEY_3] || KeysProcessed[GLFW_KEY_4] || KeysProcessed[GLFW_KEY_5]
        || KeysProcessed[GLFW_KEY_6] || KeysProcessed[GLFW_KEY_7] || KeysProcessed[GLFW_KEY_8] || KeysProcessed[GLFW_KEY_9]) return;

    if (Keys[GLFW_KEY_1]) {
        player->UseItem(0);
        KeysProcessed[GLFW_KEY_1] = true;
    }
    else if (Keys[GLFW_KEY_2]) {
        player->UseItem(1);
        KeysProcessed[GLFW_KEY_2] = true;
    }
    else if (Keys[GLFW_KEY_3]) {
        player->UseItem(2);
        KeysProcessed[GLFW_KEY_3] = true;
    }
    else if (Keys[GLFW_KEY_4]) {
        player->UseItem(3);
        KeysProcessed[GLFW_KEY_4] = true;
    }
    else if (Keys[GLFW_KEY_5]) {
        player->UseItem(4);
        KeysProcessed[GLFW_KEY_5] = true;
    }
    else if (Keys[GLFW_KEY_6]) {
        player->UseItem(5);
        KeysProcessed[GLFW_KEY_6] = true;
    }
    else if (Keys[GLFW_KEY_7]) {
        player->UseItem(6);
        KeysProcessed[GLFW_KEY_7] = true;
    }
    else if (Keys[GLFW_KEY_8]) {
        player->UseItem(7);
        KeysProcessed[GLFW_KEY_8] = true;
    }
    else if (Keys[GLFW_KEY_9]) {
        player->UseItem(8);
        KeysProcessed[GLFW_KEY_9] = true;
    }
}

void Game::ProcessInput(float dt)
{
    if (this->Keys[GLFW_KEY_ESCAPE] && !this->KeysProcessed[GLFW_KEY_ESCAPE] && (gmState == MENU || gmState == ACTIVE)) 
    {
        gmState == ACTIVE ? gmState = MENU : gmState = ACTIVE;

        this->KeysProcessed[GLFW_KEY_ESCAPE] = true;
    }

    if (gmState == ACTIVE)
    {
        // Move screen
        float midScreenX = player->GetPos().x - this->width / 2.0f + player->GetSize().x / 2.0f;
        float midScreenY = player->GetPos().y - this->height / 2.0f + player->GetSize().y / 2.0f;
        camera.cameraPos = glm::vec3(midScreenX, midScreenY, 1.0f);
        if (camera.cameraPos.x < 0.0f) camera.cameraPos.x = 0.0f;
        if (camera.cameraPos.y < 0.0f) camera.cameraPos.y = 0.0f;

        // player movement
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

        ProcessInventoryKeys();

        if (Keys[GLFW_KEY_I] && !KeysProcessed[GLFW_KEY_I]) {
            statusShow = !statusShow;
            KeysProcessed[GLFW_KEY_I] = true;
        }
    }
    else if (gmState == MENU) 
    {
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
    else if (gmState == SETTINGS) 
    {
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
        player->ProcessAction(dt);
        UpdateEnemies(dt);

        // update borders after position changes
        for (auto i : objList)
        {
            i->UpdateAABB();
        }

        if (enemyList.empty()) {
            SpawnExit();
            GenerateLevel();
            return;
        }

        // -
        UpdateAnimations(dt);
        ProcessCollisions(dt);
        ProcessDeaths();

        DeleteObjects();
    }
}

void Game::UpdateAnimations(float dt)
{ 
    for (size_t i = 0; i < animObjList.size(); i++)
    {
        animObjList[i]->PlayAnimation(dt);
    }

    for (auto ch : characterList)
    {
        ch->DamageAnimation(dt);
    }

}

void Game::ProcessCollisions(float dt)
{
    // player collisions
        // player map collision
    for (size_t i = 0; i < mapObjList.size(); i++)
    {
        if (i == 0) player->ProcessCollision((*mapObjList[i]), true, dt); // bool prevents double collision
        else player->ProcessCollision((*mapObjList[i]), false, dt);
    }
        // player attack and enemy collision
    for (auto i : enemyList)
    {
        if (player->GetAction() == ATTACK && player->AttackCollision(i)) {
            i->Hit(player->GetDamage(), player->GetAtkType());
            i->Push(player->GetPos());
        }
        else if (i->ObjectCollision(*player)) {
            i->ProcessCollision(*player, false, dt);
            player->Hit(i->GetDamage(), i->GetAtkType());
            player->Push(i->GetPos());
        }
    }
        // items
    for (auto it = itemList.begin(); it != itemList.end();) 
    {
        if (player->ObjectCollision(**it)) {
            player->AddToInventory(*it);    
            (*it)->DeleteObject(); 
            break;
        }
        else
            ++it;
    }

    // projectile collisions
    for (auto proj : projectileList)
    {
        for (auto ch : characterList)
        {
            if (proj->GetOwner() != ch && proj->ObjectCollision(*ch)) {
                ch->Hit(proj->GetDamage(), PURE);
                ch->Push(proj->GetPos());
                proj->DeleteObject();
                break;
            }
        }
    }

    // enemy collisions
        // enemy map collision
    for (size_t i = 0; i < mapObjList.size(); i++)
    {
        for (auto enemy : enemyList)
        {
            if (std::dynamic_pointer_cast<Skull>(enemy) != nullptr) // skull going throught walls
                continue;
            if (i == 0) enemy->ProcessCollision((*mapObjList[i]), true, dt);
            else enemy->ProcessCollision((*mapObjList[i]), false, dt);
        }
    }

    // projectile map
    for (size_t i = 0; i < mapObjList.size(); i++)
    {
        for (auto proj : projectileList)
        {
            if (proj->ObjectCollision(*mapObjList[i]))
                proj->DeleteObject();
        }
    }
}

void Game::ProcessDeaths()
{
    for (auto enemy : enemyList)
    {
        if (enemy->IsDead()) {
            player->LevelUp(enemy->GetExpCost());

            std::vector<std::shared_ptr<Item>> loot = enemy->GetLoot();
            if (!loot.empty()) {
                objList.insert(objList.end(), loot.begin(), loot.end());
                animObjList.insert(animObjList.end(), loot.begin(), loot.end());
                itemList.insert(itemList.end(), loot.begin(), loot.end());
            }
        }
    }

    for (auto ch : characterList)
    {
        if (ch->IsDead()) {
            ch->DeleteObject();
        }
    }
}

// game
std::vector<DropEntry> Game::GetItemsByRarity(ItemRarity rarity)
{
    std::vector<DropEntry> drop;
    DropEntry items;

    if (rarity == ItemRarity::UNIQUE) {
        items.itemID = ItemID::typeUpgrade;
        items.dropChance = 1;
        drop.push_back(items);
    }
    else if (rarity == ItemRarity::RARE)
    {
        items.itemID = ItemID::statsUpgrade;
        items.dropChance = 10;
        drop.push_back(items);

        items.itemID = ItemID::msPotion;
        items.dropChance = 15;
        drop.push_back(items);

        items.itemID = ItemID::healthPotion;
        items.dropChance = 25;
        drop.push_back(items);
    }
    
    items.itemID = ItemID::smallMsPotion;
    items.dropChance = 20;
    drop.push_back(items);

    items.itemID = ItemID::smallHealthPotion;
    items.dropChance = 30;
    drop.push_back(items);

    return drop;
}

void Game::SpawnEnemy()
{
    enum EnemyType { SKELETON = 0, SKULL, VAMP };
    EnemyType type = static_cast<EnemyType>(rand() % (VAMP + 1));
    
    // random place
    glm::vec2 enemyPos;
    while (true)
    {
        int row = rand() % (grid.size() - 2), col = rand() % (grid[0].size() - 2);
        if (grid[row][col]->data == MAINTILE && grid[row + 2][col]->data == MAINTILE && grid[row][col + 2]->data == MAINTILE && glm::distance(grid[row][col]->cellPosition, player->GetPos()) > 600.0f) {
            enemyPos = grid[row][col]->cellPosition + grid[row][col]->cellSize / 2.0f;
            grid[row][col]->data = MAINTILE_USED;
            break;
        }
    }

    std::shared_ptr<Enemy> enemy;
    if (type == SKELETON)
    {
        std::shared_ptr<Skeleton> skeleton = std::make_shared<Skeleton>(enemyPos, player->GetSize(), 175.0f, GetItemsByRarity(ItemRarity::COMMON));
        for (size_t i = 0; i < 4; i++)
        {
            skeleton->SetTexture("skeleton" + std::to_string(i));
        }
        skeleton->AddAnimation("main", 0, 4, 0.4f, true);
        enemy = skeleton;
    }
    else if (type == SKULL)
    {
        std::shared_ptr<Skull> skull = std::make_shared<Skull>(enemyPos, player->GetSize(), 150.0f, GetItemsByRarity(ItemRarity::RARE));
        for (size_t i = 0; i < 4; i++)
        {
            skull->SetTexture("skull" + std::to_string(i));
        }
        skull->AddAnimation("main", 0, 4, 0.4f, true);
        enemy = skull;
    }
    else
    {
        std::shared_ptr<Vampire> vamp = std::make_shared<Vampire>(enemyPos, player->GetSize(), 125.0f, GetItemsByRarity(ItemRarity::UNIQUE));
        for (size_t i = 0; i < 4; i++)
        {
            vamp->SetTexture("vamp" + std::to_string(i));
        }
        vamp->AddAnimation("main", 0, 4, 0.4f, true);
        vampireList.push_back(vamp);
        enemy = vamp;
    }

    objList.push_back(enemy);
    animObjList.push_back(enemy);
    characterList.push_back(enemy);
    enemyList.push_back(enemy);
}

void Game::UpdateEnemies(float dt) 
{
    for (auto i : enemyList)
    {
        i->CheckPlayer(player->GetPos());
        if (i->GetAction() == ATTACK && !player->IsDamaged())
            i->Move(player->GetPos(), dt);
    }
    for (auto i : vampireList)
    {
        if (i->GetAction() == ATTACK && !player->IsDamaged()) {
            std::shared_ptr<MagicSphere> projectile = i->Attack(player->GetPos(), dt);
            if (projectile) {
                objList.push_back(projectile);
                animObjList.push_back(projectile);
                projectileList.push_back(projectile);
            }
        }
    }
    for (auto i : projectileList)
    {
        i->Move(dt);
    }
}

void Game::SpawnExit() 
{
    exit->SetPos(glm::vec2());
    exit->UpdateAABB();
}

// render
void Game::ShowPlayerStatusBar()
{
    // Status bars
    DrawTexture(ResourceManager::GetTexture("menuTexture"), glm::vec2(25.0f), glm::vec2(250.0f, 135.0f));

    DrawTexture(ResourceManager::GetTexture("healthBarWrapTexture"), glm::vec2(60.0f, 40.0f), glm::vec2(180.0f, 50.0f));
    DrawTexture(ResourceManager::GetTexture("healthBarTexture"), glm::vec2(110.0f, 57.0f), glm::vec2(player->GetHpPercentage() * 118.0f, 15.0f));

    DrawTexture(ResourceManager::GetTexture("expBarWrapTexture"), glm::vec2(60.0f, 95.0f), glm::vec2(180.0f, 50.0f));
    DrawTexture(ResourceManager::GetTexture("expBarTexture"), glm::vec2(110.0f, 112.0f), glm::vec2(player->GetExpPercentage() * 118.0f, 15.0f));

    pixelText->RenderText(std::to_string(player->GetLvl()), glm::vec2(81.0f, 112.0f), 1.0f);

    // Effects
    if (player->GetEffect() == Effects::none) return;
    
    int duration = player->GetEffectDuration();

    if (player->GetEffect() == Effects::smallSpeedUpEffect || player->GetEffect() == Effects::speedUpEffect) {
        DrawTexture(ResourceManager::GetTexture("msPotion0"), glm::vec2(280.0f, 25.0f), glm::vec2(50.0f));
        pixelText->RenderText(std::to_string(duration), glm::vec2(325.0f, 60.0f), 0.65f);
    }
}

void Game::ShowPlayerInventory()
{
    DrawTexture(ResourceManager::GetTexture("inventoryTexture"), glm::vec2(400.0f, height - 100.0f), glm::vec2(width / 2.0f, 75.0f));

    int cnt = 0;
    for (auto& i : player->GetInventory().GetItems())
    {
        DrawTexture(ResourceManager::GetTexture(i.second.textureName), glm::vec2(435.0f + cnt * 76.0f, height - 80.0f), glm::vec2(50.0f));
        pixelText->RenderText(std::to_string(i.second.cnt), glm::vec2(470.0f + cnt * 80.0f, height - 50.0f), 0.6f);
        cnt++;
    }
}

void Game::ShowPlayerStats() 
{
    glm::vec2 size = glm::vec2(250.0f, 100.0f);
    float pos = width / 2 - size.x / 2;

    DrawTexture(ResourceManager::GetTexture("menuTexture"), glm::vec2(pos, 25.0f), size);
    pixelText->RenderText("Damage: " + std::to_string(static_cast<int>(player->GetDamage())), glm::vec2(pos + 80.0f, 50.0f), 0.6f);
    pixelText->RenderText("Armor: " + std::to_string(static_cast<int>(player->GetArmor())), glm::vec2(pos + 80.0f, 70.0f), 0.6f);
    pixelText->RenderText("Resist: " + std::to_string(static_cast<int>(player->GetResist())), glm::vec2(pos + 80.0f, 90.0f), 0.6f);
}
    // -----------
void Game::DrawTexture(Texture texture, glm::vec2 position, glm::vec2 size, float transparency, glm::vec3 colour) // menu texture
{
    ResourceManager::GetShader("menuShader").Use();
    ResourceManager::GetShader("menuShader").SetMatrix4("projection", projection);

    ResourceManager::GetShader("menuShader").SetVector3f("colour", colour);
    ResourceManager::GetShader("menuShader").SetFloat("transparency", transparency);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(size, 0.0f));

    ResourceManager::GetShader("menuShader").SetMatrix4("model", model);
    renderer->DrawTexture(texture);
}

void Game::DrawMapObject(std::vector<std::shared_ptr<MapObject>> objects)
{
    if (objects.empty()) return;

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
    if (objectVector.empty()) return;

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
    // -----------
void Game::Render()
{
    view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);

    // background/map/stats
    DrawMapObject(mainTileList);
    DrawObject(mapObjList);

    // objects
    DrawObject(enemyList);
    DrawObject(projectileList);
    DrawObject(itemList);
    DrawObject(player);

    ShowPlayerStatusBar();
    ShowPlayerInventory();
    if (statusShow) ShowPlayerStats();

#ifdef _TESTING
    dungeon->DrawDungeon();
#endif

    if (gmState == MENU) Menu();
    else if (gmState == SETTINGS) Settings();

    if (gmState != ACTIVE) DrawTexture(ResourceManager::GetTexture("cursorTexture"), glm::vec2(xMouse, yMouse), glm::vec2(30.0f, 32.0f));
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

template<typename T>
void Game::EraseFromVector(std::vector<std::shared_ptr<T>>& vector) 
{
    vector.erase(
        std::remove_if(vector.begin(), vector.end(),
            [](const std::shared_ptr<T>& obj) { return obj->IsDeleted(); }),
        vector.end()
    );
}

void Game::DeleteObjects()
{
    EraseFromVector(enemyList);
    EraseFromVector(vampireList);
    EraseFromVector(projectileList);

    EraseFromVector(itemList);

    EraseFromVector(characterList);
    EraseFromVector(animObjList);
    EraseFromVector(objList);
}

Game::~Game()
{
    delete sound;

    objList.clear();
    // -----
    menuButtons.clear();
    settingButtons.clear();
}
