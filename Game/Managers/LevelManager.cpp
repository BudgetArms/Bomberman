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
#include "Components/BombermanComponent.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/LifeComponent.hpp"
#include "Components/LifeDisplayComponent.hpp"
#include "Components/ScoreComponent.hpp"
#include "Components/ScoreDisplayComponent.hpp"
#include "Core/Text2D.hpp"


using namespace Game;


LevelManager::LevelManager()
{
    // Create Background
    // m_BackgroundSpriteSheet = std::make_unique<bae::SpriteSheet>(m_BackgroundTexturePath,
    //                                                              SDL_FRect(0, 0, 224, 1488), 1, 6);
    //
    // m_BackgroundSpriteSheet->m_Index = 0;
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

    // Score Display
    bomberman->GetComponent<ScoreDisplayComponent>()->SetPosition(glm::vec2{ 100, 540 });

    // Life Display
    bomberman->GetComponent<LifeDisplayComponent>()->m_Position = { 5, 540 };

    // Controls
    // AddControls(*msPacman, true);

    scene->Add(bomberman);
}

void LevelManager::SpawnBombermiss()
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const glm::vec2 spawnPosition = { 300, 300 };

    const auto bombermiss = GetBombermanBase("Bombermiss", spawnPosition);

    // Score Display
    bombermiss->GetComponent<ScoreDisplayComponent>()->SetPosition(glm::vec2{ 400, 540 });

    // Life Display
    bombermiss->GetComponent<LifeDisplayComponent>()->m_Position = { 300, 540 };

    // Controls
    // AddControls(*msPacman, false);

    scene->Add(bombermiss);
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
