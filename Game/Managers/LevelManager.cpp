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
#include "Core/Scene.hpp"
#include "Managers/ResourceManager.hpp"
#include "Managers/SceneManager.hpp"

#include "Base/CommonManagerVariables.hpp"
#include "Base/Events.hpp"
#include "Commands/ForceDamageCommand.hpp"
#include "Components/BombermanComponent.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/LifeComponent.hpp"
#include "Components/LifeDisplayComponent.hpp"
#include "Components/ScoreComponent.hpp"
#include "Components/ScoreDisplayComponent.hpp"
#include "Core/Renderer.hpp"
#include "Core/Text2D.hpp"
#include "Wrappers/Keyboard.hpp"


using namespace Game;


LevelManager::LevelManager()
{
    // Create Background
    m_BackgroundTexture = bae::ResourceManager::GetInstance().LoadTexture(m_BackgroundTexturePath).get();
    m_LevelBlockTest    = new bae::SpriteSheet("Textures/Level/TempBlock.png", SDL_FRect(0, 0, 112, 16), 7, 1);

    // Test
    m_LevelBlockTest->m_Scale                 = { 2, 2 };
    m_LevelBlockTest->m_Position              = { 32, 64 };
    m_LevelBlockTest->m_bIsCenteredAtPosition = false;
}

LevelManager::~LevelManager()
{
    // ClearLevel();
}


void LevelManager::SpawnBomberman()
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const glm::vec2 spawnPosition = { 200, 300 };

    const auto bomberman = GetBombermanBase("Bomberman", spawnPosition);

    // Observer
    bomberman->GetComponent<BombermanComponent>()->AddObserver(this);

    // Lives
    bomberman->GetComponent<LifeComponent>()->SetLives(m_BombermanLives);

    // Score
    bomberman->GetComponent<ScoreComponent>()->SetScore(m_BombermanScore);

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

    const glm::vec2 spawnPosition = { 300, 300 };

    const auto bombermiss = GetBombermanBase("Bombermiss", spawnPosition);

    // Observer
    bombermiss->GetComponent<BombermanComponent>()->AddObserver(this);

    // Lives
    bombermiss->GetComponent<LifeComponent>()->SetLives(m_BombermissLives);

    // Score
    bombermiss->GetComponent<ScoreComponent>()->SetScore(m_BombermissScore);

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

    const glm::vec2 spawnPosition = m_BalloomPlayerStartPosition;

    const auto balloom = GetEnemyBase("Balloom Player", spawnPosition);
    balloom->AddComponent<bae::SpriteComponent>(*balloom, "Textures/Characters/Enemies.png",
                                                SDL_FRect(0, 0, 32, 16), 2, 1);

    m_Enemies.insert(balloom.get());

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

    m_Enemies.insert(balloom.get());

    scene->Add(balloom);
}

void LevelManager::SpawnOneal(const glm::vec2& position)
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const auto oneal = GetEnemyBase("Oneal", position);
    oneal->AddComponent<bae::SpriteComponent>(*oneal, "Textures/Characters/Enemies.png",
                                              SDL_FRect(0, 16, 32, 16), 2, 1);

    m_Enemies.insert(oneal.get());
    scene->Add(oneal);
}

void LevelManager::SpawnDoll(const glm::vec2& position)
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const auto doll = GetEnemyBase("Doll", position);
    doll->AddComponent<bae::SpriteComponent>(*doll, "Textures/Characters/Enemies.png",
                                             SDL_FRect(0, 32, 32, 16), 2, 1);

    m_Enemies.insert(doll.get());
    scene->Add(doll);
}

void LevelManager::SpawnMinvo(const glm::vec2& position)
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const auto minvo = GetEnemyBase("Minvo", position);
    minvo->AddComponent<bae::SpriteComponent>(*minvo, "Textures/Characters/Enemies.png",
                                              SDL_FRect(0, 48, 32, 16), 2, 1);

    m_Enemies.insert(minvo.get());
    scene->Add(minvo);
}


std::shared_ptr<bae::GameObject> LevelManager::GetBombermanBase(const std::string& gameObjectName,
                                                                const glm::vec2& spawnPosition)
{
    const auto bomberman = std::make_shared<bae::GameObject>(gameObjectName);
    bomberman->SetWorldLocation(spawnPosition);

    bomberman->AddComponent<BombermanComponent>(*bomberman);
    const auto bombermanComp = bomberman->GetComponent<BombermanComponent>();

    constexpr glm::vec2 dimensions = { 20, 20 };
    constexpr glm::vec2 offset     = { -dimensions.x / 2.f, -dimensions.y / 2.f };

    bomberman->AddComponent<HitboxComponent>(*bomberman, dimensions, offset);
    bomberman->GetComponent<HitboxComponent>()->SetVisibility(true);
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

    // bomberman->GetComponent<GridMovementComponent>()->m_Speed = 100.f;
    // bomberman->GetComponent<GridMovementComponent>()->AddObserver(bombermanComponent);

    return bomberman;
}

std::shared_ptr<bae::GameObject> LevelManager::GetEnemyBase(const std::string& gameObjectName,
                                                            const glm::vec2& spawnPosition)
{
    const auto enemy = std::make_shared<bae::GameObject>(gameObjectName);
    enemy->SetWorldLocation(spawnPosition);

    constexpr glm::vec2 dimensions = { 20, 20 };
    constexpr glm::vec2 offset     = { -dimensions.x / 2.f, -dimensions.y / 2.f };

    enemy->AddComponent<HitboxComponent>(*enemy, dimensions, offset);
    enemy->GetComponent<HitboxComponent>()->SetVisibility(true);

    return enemy;
}

void LevelManager::RenderBackground() const
{
    bae::Renderer::GetInstance().RenderTexture(*m_BackgroundTexture, false, { 0, 0 }, 0, { 2.f, 2.f });
    m_LevelBlockTest->Render();
}

void LevelManager::SpawnBlocks()
{
}

void LevelManager::SkipLevel()
{
    SavePlayerData();

    ++m_CurrentLevel;
    RestartLevel();
}

void LevelManager::ClearLevel()
{
    const bae::Scene* scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());
    scene->RemoveAll();

    m_Bomberman  = nullptr;
    m_Bombermiss = nullptr;
}


std::set<bae::GameObject*> LevelManager::GetPlayers()
{
    switch(m_GameMode)
    {
        case GameMode::Singleplayer:
            return { m_Bomberman };
        case GameMode::CoOp:
            return { m_Bomberman, m_Bombermiss };
        case GameMode::Versus:
            return { m_Bomberman };
    }

    std::cout << FUNCTION_NAME << "This should never be reached" << '\n';
    return { nullptr };
}

std::set<bae::GameObject*> LevelManager::GetEnemies()
{
    return m_Enemies;
}


void LevelManager::HandleEvent(const unsigned int eventHash)
{
    switch(GetEvent(eventHash))
    {
        case Events::PlayerDied:
            break;
        case Events::DirectionChanged:
            break;
        case Events::GameWon:
            break;
        case Events::GameOver:
            break;
        case Events::LevelWon:
            break;
        case Events::LevelLost:
            break;
        case Events::BalloomDied:
            break;
        case Events::OnealDied:
            break;
        case Events::DollDied:
            break;
        case Events::MinvoDied:
            break;
        case Events::BeginLevel:
            break;
        case Events::RestartLevel:
            break;
        case Events::ScoreChanged:
            break;
        case Events::LivesChanged:
            break;
        case Events::CollisionEvent:
            break;
        case Events::NoEvent:
            break;
    }
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


void LevelManager::RestartLevel()
{
    ClearLevel();

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
}


void LevelManager::HandleGameOver() const
{
    int score{};

    // Get Score
    switch(m_GameMode)
    {
        case GameMode::Singleplayer:
        {
            score = m_BombermanScore;
            std::cout << FUNCTION_NAME << ": SinglePlayer, Score: " << score << '\n';
        }
        break;
        case GameMode::CoOp:
        {
            score = m_BombermanScore + m_BombermissScore;
            std::cout << FUNCTION_NAME << ": Co-Op, Score: " << score << '\n';
        }
        break;
        case GameMode::Versus:
        {
            score = m_BombermanScore;
            std::cout << FUNCTION_NAME << ": Versus, Score: " << score << '\n';
        }
        break;
    }

    // Load Input Name Scene
}

void LevelManager::AddControls([[maybe_unused]] const bae::GameObject& gameObject, const bool bIsFirstPlayer) const
{
    // todo: remove maybe_unused
    [[maybe_unused]] const bae::Keyboard& keyboard = bae::InputManager::GetInstance().GetKeyboard();
    const bae::Controller* controller              = bae::InputManager::GetInstance().GetController(!bIsFirstPlayer);

    [[maybe_unused]] constexpr auto moveOnGridButtonState = bae::InputManager::ButtonState::Pressed;

    if(!controller)
    {
        std::cout << FUNCTION_NAME << " Failed to Get controller, IsFirstPlayer: "
                << std::boolalpha << bIsFirstPlayer << '\n';
        return;
    }

    // auto moveOnGridLeftCommand  = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Left);
    // auto moveOnGridRightCommand = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Right);
    // auto moveOnGridDownCommand  = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Down);
    // auto moveOnGridUpCommand    = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Up);

    // [[maybe_unused]] auto moveOnGridLeftCommand  = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Left);
    // [[maybe_unused]] auto moveOnGridRightCommand = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Right);
    // [[maybe_unused]] auto moveOnGridDownCommand  = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Down);
    // [[maybe_unused]] auto moveOnGridUpCommand    = std::make_unique<MoveOnGridCommand>(gameObject, Direction::Up);


    if(bIsFirstPlayer)
    {
        // keyboard.AddKeyboardCommands(std::move(moveOnGridLeftCommand), SDLK_A, moveOnGridButtonState);
        // keyboard.AddKeyboardCommands(std::move(moveOnGridRightCommand), SDLK_D, moveOnGridButtonState);
        // keyboard.AddKeyboardCommands(std::move(moveOnGridDownCommand), SDLK_S, moveOnGridButtonState);
        // keyboard.AddKeyboardCommands(std::move(moveOnGridUpCommand), SDLK_W, moveOnGridButtonState);
    }
    else
    {
        // keyboard.AddKeyboardCommands(std::move(moveOnGridLeftCommand), SDLK_LEFT, moveOnGridButtonState);
        // keyboard.AddKeyboardCommands(std::move(moveOnGridRightCommand), SDLK_RIGHT, moveOnGridButtonState);
        // keyboard.AddKeyboardCommands(std::move(moveOnGridDownCommand), SDLK_DOWN, moveOnGridButtonState);
        // keyboard.AddKeyboardCommands(std::move(moveOnGridUpCommand), SDLK_UP, moveOnGridButtonState);
    }

    // #if WIN32
    // controller->AddControllerCommands(std::move(moveOnGridLeftCommand), XINPUT_GAMEPAD_DPAD_LEFT,
    //                                   moveOnGridButtonState);
    // controller->AddControllerCommands(std::move(moveOnGridRightCommand), XINPUT_GAMEPAD_DPAD_RIGHT,
    //                                   moveOnGridButtonState);
    // controller->AddControllerCommands(std::move(moveOnGridDownCommand), XINPUT_GAMEPAD_DPAD_DOWN,
    //                                   moveOnGridButtonState);
    // controller->AddControllerCommands(std::move(moveOnGridUpCommand), XINPUT_GAMEPAD_DPAD_UP, moveOnGridButtonState);
    // #endif
}

void LevelManager::SavePlayerData()
{
    // Save lives & Score
    m_BombermanLives = m_Bomberman->GetComponent<LifeComponent>()->GetLives();
    m_BombermanScore = m_Bomberman->GetComponent<ScoreComponent>()->GetScore();

    if(m_Bombermiss)
    {
        m_BombermissLives = m_Bombermiss->GetComponent<LifeComponent>()->GetLives();
        m_BombermissScore = m_Bombermiss->GetComponent<ScoreComponent>()->GetScore();
    }
}

void LevelManager::LoadPlayerData() const
{
    m_Bomberman->GetComponent<LifeComponent>()->SetLives(m_BombermanLives);
    m_Bomberman->GetComponent<ScoreComponent>()->SetScore(m_BombermanScore);

    if(m_Bombermiss)
    {
        m_Bombermiss->GetComponent<LifeComponent>()->SetLives(m_BombermissLives);
        m_Bombermiss->GetComponent<ScoreComponent>()->SetScore(m_BombermissScore);
    }
}
