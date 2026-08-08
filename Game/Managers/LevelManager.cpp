#include "LevelManager.hpp"

#if WIN32
#include <Windows.h>
#include <XInput.h>
#endif

#include <fstream>

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "Components/SpriteComponent.hpp"
#include "Core/HelperFunctions.hpp"
#include "Core/Renderer.hpp"
#include "Core/Scene.hpp"
#include "Core/Text2D.hpp"
#include "Managers/ResourceManager.hpp"
#include "Managers/SceneManager.hpp"
#include "Wrappers/Controller.hpp"
#include "Wrappers/Keyboard.hpp"

#include "Base/CommonManagerVariables.hpp"
#include "Base/Events.hpp"
#include "Commands/ForceDamageCommand.hpp"
#include "Commands/MoveCommand.hpp"
#include "Components/BombermanComponent.hpp"
#include "Components/DoorComponent.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/LifeComponent.hpp"
#include "Components/LifeDisplayComponent.hpp"
#include "Components/MovementGridComponent.hpp"
#include "Components/ScoreComponent.hpp"
#include "Components/ScoreDisplayComponent.hpp"
#include "Components/TemporaryWallComponent.hpp"


using namespace Game;


LevelManager::LevelManager()
{
    // Create Background
    m_BackgroundTexture = bae::ResourceManager::GetInstance().LoadTexture(m_BackgroundTexturePath).get();
}

LevelManager::~LevelManager()
{
    // ClearLevel();
}

void LevelManager::StartGame(const GameMode gameMode)
{
    if(m_bHasGameStarted)
    {
        std::cout << FUNCTION_NAME << ": Cannot start Game that's already started" << '\n';
        return;
    }

    m_GameMode        = gameMode;
    m_bHasGameStarted = true;

    // Clear all Data
    m_CurrentLevel = 0;
    m_Bomberman    = nullptr;
    m_Bombermiss   = nullptr;
    m_Enemies.clear();

    LoadStartLevelData();

    CreateGrid();

    AddPermanentWalls();

    SpawnDoor(ToPosition(m_DoorPosition));

    AddTemporaryWalls();

    RestartLevel();
}


void LevelManager::RenderBackground() const
{
    if(m_bHasGameStarted)
    {
        constexpr bool isCenteredAtPosition{ false };
        constexpr glm::vec2 position{ 0.f, 0.f };
        constexpr float angle{ 0.f };
        constexpr glm::vec2 scale = { m_GlobalScale, m_GlobalScale };

        bae::Renderer::GetInstance().RenderTexture(*m_BackgroundTexture, isCenteredAtPosition, position, angle, scale);
    }
}

void LevelManager::SkipLevel()
{
    SavePlayerData();

    ++m_CurrentLevel;
    RestartLevel();
}

std::vector<std::pair<bae::GameObject*, PlayerType>> LevelManager::GetPlayers()
{
    switch(m_GameMode)
    {
        case GameMode::Singleplayer:
            return { { m_Bomberman, PlayerType::Bomberman } };
        case GameMode::CoOp:
            return {
                { m_Bomberman, PlayerType::Bomberman },
                { m_Bombermiss, PlayerType::Bombermiss }
            };
        case GameMode::Versus:
            return { { m_Bomberman, PlayerType::Bomberman } };
    }

    std::cout << FUNCTION_NAME << "This should never be reached" << '\n';
    return { { nullptr, PlayerType::Bomberman } };
}

std::unordered_map<bae::GameObject*, EnemyType> LevelManager::GetEnemies()
{
    return m_Enemies;
}

GameMode LevelManager::GetGameMode() const
{
    return m_GameMode;
}

int LevelManager::GetTotalScore()
{
    SavePlayerData();
    switch(m_GameMode)
    {
        case GameMode::Singleplayer:
            return m_BombermanInfo.Score;
        case GameMode::CoOp:
            return m_BombermanInfo.Score + m_BombermissInfo.Score;
        case GameMode::Versus:
            return m_BombermanInfo.Score;
    }

    return -1;
}

GridComponent* LevelManager::GetGridComponent() const
{
    return m_GridComponent;
}

void LevelManager::HandleEvent(const unsigned int)
{
}


void LevelManager::Notify(const unsigned eventHash, bae::Subject* subject, const std::any&)
{
    if(GetEvent(eventHash) == Events::PlayerDied)
    {
        std::cout << FUNCTION_NAME << '\t';
        if(subject->GetGameObject() == m_Bomberman ||
            subject->GetGameObject() == m_Bombermiss)
        {
            HandleBomberDeath(*subject->GetGameObject());
        }
    }
}

void LevelManager::HandleBomberDeath(const bae::GameObject& object)
{
    if(&object != m_Bomberman && &object != m_Bombermiss)
    {
        std::cout << FUNCTION_NAME << " This should never be reached" << '\n';
        return;
    }

    SavePlayerData();

    if(object.GetComponent<LifeComponent>()->IsAlive())
    {
        RestartLevel();
    }
    else
    {
        HandleGameOver();
    }
}

void LevelManager::HandleGameOver() const
{
    // Load Input Name Scene
}

void LevelManager::ClearLevel()
{
    const bae::Scene* scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());
    scene->RemoveAll();

    m_Bomberman  = nullptr;
    m_Bombermiss = nullptr;

    m_BombermanInfo = {};
}

void LevelManager::RestartLevel()
{
    ClearLevel();

    // Spawn Temporary Blocks

    // Spawn Door

    // Level Generation

    // Spawn Player(s)
    switch(m_GameMode)
    {
        case GameMode::Singleplayer:
            SpawnBomberman();
            break;
        case GameMode::CoOp:
            SpawnBomberman();
            SpawnBombermiss();
            break;
        case GameMode::Versus:
            SpawnBomberman();
            SpawnBalloomPlayer();
            break;
    }

    // Spawn Enemies
    for(const auto [enemyType, position] : m_EnemyStartPositions)
    {
        switch(enemyType)
        {
            case EnemyType::Balloom:
                SpawnBalloom(ToPosition(position));
                break;
            case EnemyType::Oneal:
                SpawnOneal(ToPosition(position));
                break;
            case EnemyType::Doll:
                SpawnDoll(ToPosition(position));
                break;
            case EnemyType::Minvo:
                SpawnMinvo(ToPosition(position));
                break;
            case EnemyType::BalloomPlayer:
                std::cout << "This shouldn't be reached" << '\n';
                break;
        }
    }
}

void LevelManager::LoadLevelInfo(const std::filesystem::path& jsonFile)
{
    const auto resourceFolder = bae::ResourceManager::GetInstance().GetResourcesPath();
    if(!std::filesystem::exists(resourceFolder / jsonFile))
    {
        throw std::runtime_error(
            FUNCTION_NAME + std::string(" Failed! File Not found, file") + jsonFile.string());
    }

    std::ifstream file{ resourceFolder / jsonFile, std::ios::binary };
    if(!file.is_open())
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed To Open File: ") + jsonFile.string());
    }

    nlohmann::json json{};
    LevelInfo levelInfo{};

    try
    {
        file >> json;
        levelInfo = json.get<LevelInfo>();
    }
    catch(const std::exception& exception)
    {
        std::cout << FUNCTION_NAME << " Failed To Parse Json File! Exception: " << exception.what() << '\n';

        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed To Parse Json File! Exception: ")
            + std::string(exception.what()));
    }

    m_LoadedLevels.insert({ levelInfo.Index, levelInfo });
}

void LevelManager::CreateGrid()
{
    auto* backgroundScene = bae::SceneManager::GetInstance().GetScene(g_LevelBackgroundName.data());

    const auto gridObject = std::make_shared<bae::GameObject>("Grid Object");
    gridObject->SetWorldLocation(m_GridInfo.Offset);

    gridObject->AddComponent<GridComponent>(*gridObject, m_GridInfo.NrColumns, m_GridInfo.NrRows, m_GridInfo.CellSize);

    m_GridComponent = gridObject->GetComponent<GridComponent>();
    m_GridComponent->SetRenderConnections(true);

    backgroundScene->Add(gridObject);
}

void LevelManager::AddPermanentWalls() const
{
    for(const bae::Graphs::GridPosition position : m_PermanentBlockPositions)
    {
        m_GridComponent->RemoveNode(position);
    }
}

void LevelManager::AddTemporaryWalls()
{
    for(const bae::Graphs::GridPosition gridPosition : m_TemporaryBlockPositions)
    {
        const glm::vec2 temporaryBlockPosition = ToPosition(gridPosition);

        SpawnTemporaryWall(temporaryBlockPosition);
    }
}


void LevelManager::SpawnBomberman()
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const glm::vec2 spawnPosition = ToPosition(m_BombermanInfo.StartPosition);

    const auto bomberman = GetBombermanBase("Bomberman", spawnPosition);

    // Observer
    bomberman->GetComponent<BombermanComponent>()->AddObserver(this);

    // Lives
    bomberman->GetComponent<LifeComponent>()->SetLives(m_BombermanInfo.Lives);

    // Score
    bomberman->GetComponent<ScoreComponent>()->SetScore(m_BombermanInfo.Score);

    // Life Display
    bomberman->GetComponent<LifeDisplayComponent>()->m_Position = { 6, 416 };

    // Score Display
    bomberman->GetComponent<ScoreDisplayComponent>()->SetPosition(glm::vec2{ 176, 432 });

    // Set Bomberman
    m_Bomberman = bomberman.get();

    // Controls
    AddControls(*bomberman, true);

    const bae::Keyboard& keyboard = bae::InputManager::GetInstance().GetKeyboard();

    auto command = std::make_unique<ForceDamageCommand>(*bomberman.get());
    keyboard.AddKeyboardCommands(std::move(command), SDLK_6, bae::InputManager::ButtonState::Down);

    scene->Add(bomberman);
}

void LevelManager::SpawnBombermiss()
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const glm::vec2 spawnPosition = ToPosition(m_BombermissInfo.StartPosition);

    const auto bombermiss = GetBombermanBase("Bombermiss", spawnPosition);

    // Observer
    bombermiss->GetComponent<BombermanComponent>()->AddObserver(this);

    // Lives
    bombermiss->GetComponent<LifeComponent>()->SetLives(m_BombermissInfo.Lives);

    // Score
    bombermiss->GetComponent<ScoreComponent>()->SetScore(m_BombermissInfo.Score);

    // Life Display
    bombermiss->GetComponent<LifeDisplayComponent>()->m_Position = { 890, 416 };

    // Score Display
    bombermiss->GetComponent<ScoreDisplayComponent>()->SetPosition(glm::vec2{ 810, 432 });


    // Set Bombermiss
    m_Bombermiss = bombermiss.get();

    // Controls
    AddControls(*bombermiss, false);

    scene->Add(bombermiss);
}

void LevelManager::SpawnBalloomPlayer()
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const glm::vec2 spawnPosition = ToPosition(m_BalloomPlayerInfo.StartPosition);

    const auto balloom = GetEnemyBase("Balloom Player", spawnPosition);
    balloom->AddComponent<bae::SpriteComponent>(*balloom, "Textures/Characters/Enemies.png",
                                                SDL_FRect(0, 0, 32, 16), 2, 1);

    m_Enemies.insert({ balloom.get(), EnemyType::BalloomPlayer });

    // Controls
    AddControls(*balloom, false);

    scene->Add(balloom);
}


void LevelManager::SpawnBalloom(const glm::vec2& position)
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const auto balloom = GetEnemyBase("Balloom", position);
    balloom->AddComponent<bae::SpriteComponent>(*balloom, "Textures/Characters/Enemies.png",
                                                SDL_FRect(0, 0, 32, 16), 2, 1);

    m_Enemies.insert({ balloom.get(), EnemyType::Balloom });

    scene->Add(balloom);
}

void LevelManager::SpawnOneal(const glm::vec2& position)
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const auto oneal = GetEnemyBase("Oneal", position);
    oneal->AddComponent<bae::SpriteComponent>(*oneal, "Textures/Characters/Enemies.png",
                                              SDL_FRect(0, 16, 32, 16), 2, 1);

    m_Enemies.insert({ oneal.get(), EnemyType::Oneal });
    scene->Add(oneal);
}

void LevelManager::SpawnDoll(const glm::vec2& position)
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const auto doll = GetEnemyBase("Doll", position);
    doll->AddComponent<bae::SpriteComponent>(*doll, "Textures/Characters/Enemies.png",
                                             SDL_FRect(0, 32, 32, 16), 2, 1);

    m_Enemies.insert({ doll.get(), EnemyType::Doll });
    scene->Add(doll);
}

void LevelManager::SpawnMinvo(const glm::vec2& position)
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const auto minvo = GetEnemyBase("Minvo", position);
    minvo->AddComponent<bae::SpriteComponent>(*minvo, "Textures/Characters/Enemies.png",
                                              SDL_FRect(0, 48, 32, 16), 2, 1);

    m_Enemies.insert({ minvo.get(), EnemyType::Minvo });
    scene->Add(minvo);
}


std::shared_ptr<bae::GameObject> LevelManager::GetBombermanBase(const std::string& gameObjectName,
                                                                const glm::vec2& spawnPosition)
{
    const auto bomberman = std::make_shared<bae::GameObject>(gameObjectName);
    bomberman->SetWorldLocation(spawnPosition);
    bomberman->SetWorldScale({ m_GlobalScale, m_GlobalScale });

    bomberman->AddComponent<BombermanComponent>(*bomberman);
    const auto bombermanComp = bomberman->GetComponent<BombermanComponent>();

    const glm::vec2 offset = -m_HitboxDimension / 2.f;

    bomberman->AddComponent<HitboxComponent>(*bomberman, m_HitboxDimension, offset);
    bomberman->GetComponent<HitboxComponent>()->SetVisibility(false);
    bomberman->GetComponent<HitboxComponent>()->AddObserver(bombermanComp);

    // Score Display
    auto text = std::make_unique<bae::Text2D>("XXX");

    text->m_bIsCenteredAtPosition = true;
    bomberman->AddComponent<ScoreDisplayComponent>(*bomberman, glm::vec2{ 0, 0 }, std::move(text));

    const auto scoreDisplayComp = bomberman->GetComponent<ScoreDisplayComponent>();
    bomberman->GetComponent<ScoreComponent>()->AddObserver(scoreDisplayComp);

    // Life Display
    auto lifeTexture = bae::ResourceManager::GetInstance().LoadTexture("Textures/Level/Heart.png");
    bomberman->AddComponent<LifeDisplayComponent>(*bomberman, glm::vec2{ 0, 0 }, lifeTexture);
    bomberman->GetComponent<LifeDisplayComponent>()->m_Scale = { 1.f, 1.f };

    const auto lifeDisplayComp = bomberman->GetComponent<LifeDisplayComponent>();
    bomberman->GetComponent<LifeComponent>()->AddObserver(lifeDisplayComp);

    return bomberman;
}


std::shared_ptr<bae::GameObject> LevelManager::GetEnemyBase(const std::string& gameObjectName,
                                                            const glm::vec2& spawnPosition)
{
    const auto enemy = std::make_shared<bae::GameObject>(gameObjectName);
    enemy->SetWorldLocation(spawnPosition);
    enemy->SetWorldScale({ m_GlobalScale, m_GlobalScale });

    const glm::vec2 offset = -m_HitboxDimension / 2.f;

    enemy->AddComponent<HitboxComponent>(*enemy, m_HitboxDimension, offset);
    enemy->GetComponent<HitboxComponent>()->SetVisibility(false);

    return enemy;
}

void LevelManager::SpawnTemporaryWall(const glm::vec2& position)
{
    bae::Scene* scene = bae::SceneManager::GetInstance().GetScene(g_LevelBackgroundName.data());

    const auto temporaryWall = std::make_shared<bae::GameObject>("Temporary Wall");
    temporaryWall->SetWorldLocation(position);
    temporaryWall->SetWorldScale({ m_GlobalScale, m_GlobalScale });

    const glm::vec2 offset = -m_HitboxDimension / 2.f;

    temporaryWall->AddComponent<HitboxComponent>(*temporaryWall, m_HitboxDimension, offset);
    temporaryWall->GetComponent<HitboxComponent>()->SetVisibility(false);

    temporaryWall->AddComponent<TemporaryWallComponent>(*temporaryWall);

    scene->Add(temporaryWall);
}

void LevelManager::SpawnDoor(const glm::vec2& position)
{
    bae::Scene* scene = bae::SceneManager::GetInstance().GetScene(g_LevelBackgroundName.data());

    const auto door = std::make_shared<bae::GameObject>("Door");
    door->SetWorldLocation(position);
    door->SetWorldScale({ m_GlobalScale, m_GlobalScale });

    const glm::vec2 offset = -m_HitboxDimension / 2.f;

    door->AddComponent<HitboxComponent>(*door, m_HitboxDimension, offset);
    door->GetComponent<HitboxComponent>()->SetVisibility(false);

    door->AddComponent<DoorComponent>(*door);

    scene->Add(door);
}

void LevelManager::AddControls(bae::GameObject& gameObject, const bool bIsFirstPlayer)
{
    const bae::Keyboard& keyboard     = bae::InputManager::GetInstance().GetKeyboard();
    const bae::Controller* controller = bae::InputManager::GetInstance().GetController(!bIsFirstPlayer);

    constexpr auto moveOnGridButtonState = bae::InputManager::ButtonState::Pressed;

    if(!controller)
    {
        std::cout << FUNCTION_NAME << " Failed to Get controller, IsFirstPlayer: "
                << std::boolalpha << bIsFirstPlayer << '\n';
        return;
    }

    gameObject.AddComponent<MovementGridComponent>(gameObject);

    auto keyboardMoveOnGridLeftCommand  = std::make_unique<MoveCommand>(gameObject, Direction::Left);
    auto keyboardMoveOnGridRightCommand = std::make_unique<MoveCommand>(gameObject, Direction::Right);
    auto keyboardMoveOnGridDownCommand  = std::make_unique<MoveCommand>(gameObject, Direction::Down);
    auto keyboardMoveOnGridUpCommand    = std::make_unique<MoveCommand>(gameObject, Direction::Up);

    if(bIsFirstPlayer)
    {
        keyboard.AddKeyboardCommands(std::move(keyboardMoveOnGridLeftCommand), SDLK_A, moveOnGridButtonState);
        keyboard.AddKeyboardCommands(std::move(keyboardMoveOnGridRightCommand), SDLK_D, moveOnGridButtonState);
        keyboard.AddKeyboardCommands(std::move(keyboardMoveOnGridDownCommand), SDLK_S, moveOnGridButtonState);
        keyboard.AddKeyboardCommands(std::move(keyboardMoveOnGridUpCommand), SDLK_W, moveOnGridButtonState);
    }
    else
    {
        keyboard.AddKeyboardCommands(std::move(keyboardMoveOnGridLeftCommand), SDLK_LEFT, moveOnGridButtonState);
        keyboard.AddKeyboardCommands(std::move(keyboardMoveOnGridRightCommand), SDLK_RIGHT, moveOnGridButtonState);
        keyboard.AddKeyboardCommands(std::move(keyboardMoveOnGridDownCommand), SDLK_DOWN, moveOnGridButtonState);
        keyboard.AddKeyboardCommands(std::move(keyboardMoveOnGridUpCommand), SDLK_UP, moveOnGridButtonState);
    }

    [[maybe_unused]] auto controllerMoveOnGridLeftCommand  = std::make_unique<MoveCommand>(gameObject, Direction::Left);
    [[maybe_unused]] auto controllerMoveOnGridRightCommand = std::make_unique<MoveCommand>(gameObject,
        Direction::Right);
    [[maybe_unused]] auto controllerMoveOnGridDownCommand = std::make_unique<MoveCommand>(gameObject, Direction::Down);
    [[maybe_unused]] auto controllerMoveOnGridUpCommand   = std::make_unique<MoveCommand>(gameObject, Direction::Up);

    #if WIN32
    controller->AddControllerCommands(std::move(controllerMoveOnGridLeftCommand), XINPUT_GAMEPAD_DPAD_LEFT,
                                      moveOnGridButtonState);
    controller->AddControllerCommands(std::move(controllerMoveOnGridRightCommand), XINPUT_GAMEPAD_DPAD_RIGHT,
                                      moveOnGridButtonState);
    controller->AddControllerCommands(std::move(controllerMoveOnGridDownCommand), XINPUT_GAMEPAD_DPAD_DOWN,
                                      moveOnGridButtonState);
    controller->AddControllerCommands(std::move(controllerMoveOnGridUpCommand), XINPUT_GAMEPAD_DPAD_UP,
                                      moveOnGridButtonState);
    #endif
}

void LevelManager::SavePlayerData()
{
    // Save lives & Score
    m_BombermanInfo.Lives = m_Bomberman->GetComponent<LifeComponent>()->GetLives();
    m_BombermanInfo.Score = m_Bomberman->GetComponent<ScoreComponent>()->GetScore();

    if(m_Bombermiss)
    {
        m_BombermissInfo.Lives = m_Bombermiss->GetComponent<LifeComponent>()->GetLives();
        m_BombermissInfo.Score = m_Bombermiss->GetComponent<ScoreComponent>()->GetScore();
    }
}

void LevelManager::LoadPlayerData() const
{
    m_Bomberman->GetComponent<LifeComponent>()->SetLives(m_BombermanInfo.Lives);
    m_Bomberman->GetComponent<ScoreComponent>()->SetScore(m_BombermanInfo.Score);

    if(m_Bombermiss)
    {
        m_Bombermiss->GetComponent<LifeComponent>()->SetLives(m_BombermissInfo.Lives);
        m_Bombermiss->GetComponent<ScoreComponent>()->SetScore(m_BombermissInfo.Score);
    }
}

void LevelManager::LoadStartLevelData()
{
    if(!m_LoadedLevels.contains(0))
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed To Find Level With Level Index: ")
            + std::to_string(0));
    }

    const LevelInfo levelInfo = m_LoadedLevels[0];

    m_GridInfo =
    {
        .NrColumns = levelInfo.GridNrColumns,
        .NrRows    = levelInfo.GridNrRows,
        .CellSize  = levelInfo.GridCellSize,
        .Offset    = levelInfo.GridOffset,
    };

    m_HitboxDimension = levelInfo.HitboxDimensions;

    m_BombermanInfo =
    {
        .StartPosition = levelInfo.BombermanPosition,
        .Lives         = levelInfo.BombermanStartLives,
        .Speed         = levelInfo.BombermanSpeed,
        .Score         = 0
    };

    m_BombermissInfo =
    {
        .StartPosition = levelInfo.BombermissPosition,
        .Lives         = levelInfo.BombermissStartLives,
        .Speed         = levelInfo.BombermissSpeed,
        .Score         = 0
    };

    m_BalloomPlayerInfo =
    {
        .StartPosition = levelInfo.BalloomPlayerPosition,
        .Lives         = 0,
        .Speed         = levelInfo.BalloomSpeed,
        .Score         = 0
    };


    m_EnemySharedInfos =
    {
        {
            EnemyType::Balloom,
            SharedEnemyInfo
            {
                .Speed             = levelInfo.BalloomSpeed,
                .DirectionUpChance = levelInfo.BalloomDirectionUpChance
            }
        },
        {
            EnemyType::Oneal,
            SharedEnemyInfo
            {
                .Speed             = levelInfo.OnealSpeed,
                .DirectionUpChance = levelInfo.OnealDirectionUpChance
            }
        },
        {
            EnemyType::Doll,
            SharedEnemyInfo
            {
                .Speed             = levelInfo.DollSpeed,
                .DirectionUpChance = levelInfo.DollDirectionUpChance
            }
        },
        {
            EnemyType::Minvo,
            SharedEnemyInfo
            {
                .Speed             = levelInfo.MinvoSpeed,
                .DirectionUpChance = levelInfo.MinvoDirectionUpChance
            }
        },
    };

    m_DoorPosition = levelInfo.DoorPosition;

    if(levelInfo.PickupBombPosition != bae::Graphs::GridPosition{})
    {
        m_PickupPosition.insert({ PickupType::Bomb, levelInfo.PickupBombPosition });
    }

    if(levelInfo.PickupFirePosition != bae::Graphs::GridPosition{})
    {
        m_PickupPosition.insert({ PickupType::Fire, levelInfo.PickupFirePosition });
    }

    if(levelInfo.PickupRemoteControlPosition != bae::Graphs::GridPosition{})
    {
        m_PickupPosition.insert({ PickupType::RemoteControl, levelInfo.PickupRemoteControlPosition });
    }


    // Get Enemy Positions
    auto InsertEnemyPosition = [&](const bae::Graphs::GridPosition gridPosition, EnemyType enemyType)
    {
        m_EnemyStartPositions.emplace_back(enemyType, gridPosition);
    };


    for(const bae::Graphs::GridPosition balloomGridPosition : levelInfo.BalloomPositions)
    {
        InsertEnemyPosition(balloomGridPosition, EnemyType::Balloom);
    }

    for(const bae::Graphs::GridPosition onealGridPosition : levelInfo.OnealPositions)
    {
        InsertEnemyPosition(onealGridPosition, EnemyType::Oneal);
    }

    for(const bae::Graphs::GridPosition dollGridPosition : levelInfo.DollPositions)
    {
        InsertEnemyPosition(dollGridPosition, EnemyType::Doll);
    }

    for(const bae::Graphs::GridPosition minvoGridPosition : levelInfo.MinvoPositions)
    {
        InsertEnemyPosition(minvoGridPosition, EnemyType::Minvo);
    }

    m_PermanentBlockPositions = levelInfo.PermanentBlockPositions;
    m_TemporaryBlockPositions = levelInfo.TemporaryBlockPositions;
}

glm::vec2 LevelManager::ToPosition(const bae::Graphs::GridPosition gridPosition) const
{
    return m_GridComponent->GetPosition(gridPosition);
}

