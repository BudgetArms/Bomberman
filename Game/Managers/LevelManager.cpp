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

    // Score Display
    bomberman->GetComponent<ScoreDisplayComponent>()->SetPosition(glm::vec2{ 100, 540 });

    // Life Display
    bomberman->GetComponent<LifeDisplayComponent>()->m_Position = { 5, 540 };

    // Controls
    // AddControls(*bomberman, true);

    // Set Bomberman
    m_Bomberman = bomberman.get();

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

    // Score Display
    bombermiss->GetComponent<ScoreDisplayComponent>()->SetPosition(glm::vec2{ 400, 540 });

    // Life Display
    bombermiss->GetComponent<LifeDisplayComponent>()->m_Position = { 300, 540 };

    // Controls
    // AddControls(*bombermiss, false);

    // Set Bombermiss
    m_Bombermiss = bombermiss.get();

    scene->Add(bombermiss);
}

void LevelManager::SpawnBalloom(const glm::vec2& position)
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    auto balloom = GetEnemyBase("Balloom", position);
    balloom->AddComponent<bae::SpriteComponent>(*balloom, "Textures/Characters/Enemies.png",
                                                SDL_FRect(0, 0, 32, 16), 2, 1);

    m_Enemies.push_back(balloom.get());

    scene->Add(balloom);
}

void LevelManager::SpawnOneal(const glm::vec2& position)
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    auto oneal = GetEnemyBase("Oneal", position);
    oneal->AddComponent<bae::SpriteComponent>(*oneal, "Textures/Characters/Enemies.png",
                                              SDL_FRect(0, 16, 32, 16), 2, 1);

    m_Enemies.push_back(oneal.get());
    scene->Add(oneal);
}

void LevelManager::SpawnDoll(const glm::vec2& position)
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    auto doll = GetEnemyBase("Doll", position);
    doll->AddComponent<bae::SpriteComponent>(*doll, "Textures/Characters/Enemies.png",
                                             SDL_FRect(0, 32, 32, 16), 2, 1);

    m_Enemies.push_back(doll.get());
    scene->Add(doll);
}

void LevelManager::SpawnMinvo(const glm::vec2& position)
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    auto minvo = GetEnemyBase("Minvo", position);
    minvo->AddComponent<bae::SpriteComponent>(*minvo, "Textures/Characters/Enemies.png",
                                              SDL_FRect(0, 48, 32, 16), 2, 1);

    m_Enemies.push_back(minvo.get());
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
    auto text = std::make_unique<bae::Text2D>("Score: 0");
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
}

void LevelManager::ClearLevel()
{
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

void LevelManager::HandleBomberDeath(const bae::GameObject& object)
{
    if(&object != m_Bomberman || &object != m_Bombermiss)
    {
        std::cout << FUNCTION_NAME << " This should never be reached" << '\n';
        return;
    }

    if(object.GetComponent<LifeComponent>()->IsAlive())
    {
        RestartLevel();
    }
    else
    {
        HandleGameOver();
    }
}

void LevelManager::RespawnPlayer(bae::GameObject&)
{
    // const int lives = player.GetComponent<LifeComponent>()->GetLives();
}

void LevelManager::RestartLevel()
{
    ClearLevel();
}

void LevelManager::HandleGameOver()
{
    switch(m_GameMode)
    {
        case GameMode::Singleplayer:
            break;
        case GameMode::CoOp:
            break;
        case GameMode::Versus:
            break;
    }
}


void LevelManager::Notify(const unsigned eventHash, bae::Subject* subject, const std::any&)
{
    if(GetEvent(eventHash) == Events::LivesChanged)
    {
        std::cout << FUNCTION_NAME << '\t';
        if(subject->GetGameObject() == m_Bomberman ||
            subject->GetGameObject() == m_Bombermiss)
        {
            HandleBomberDeath(*subject->GetGameObject());
        }
    }
}
