#include "BombHandler.hpp"

#include "CommonManagerVariables.hpp"
#include "Components/BombComponent.hpp"
#include "Components/HitboxComponent.hpp"
#include "Core/HelperFunctions.hpp"
#include "Core/Scene.hpp"
#include "Managers/LevelManager.hpp"
#include "Managers/SceneManager.hpp"


using namespace Game;


BombHandler::BombHandler(bae::GameObject& owner) :
    m_BombHandlerOwner{ &owner }
{
}

void BombHandler::UpdateBombs()
{
    if(!m_bIsEnabled)
    {
        return;
    }

    // erase placed bomb is GameObject is invalid,
    // could use events for this, but time constraint
    std::erase_if(m_PlacedBombs, [](const bae::GameObject* bombObject)
    {
        return !bombObject || bombObject->IsMarkedForDeletion();
    });
}

void BombHandler::TryPlaceBomb()
{
    if(!m_bIsEnabled)
    {
        return;
    }

    if(m_PlacedBombs.size() >= LevelManager::GetInstance().GetNrBombsAllowContinuously())
    {
        return;
    }

    PlaceBomb();
}

void BombHandler::TryActivateBomb()
{
    if(!m_bIsEnabled)
    {
        return;
    }

    if(!LevelManager::GetInstance().HasBombRemoteControl())
    {
        return;
    }

    if(m_PlacedBombs.empty())
    {
        return;
    }

    if(!m_PlacedBombs[0])
    {
        std::cout << FUNCTION_NAME << " Invalid Bomb" << '\n';
        return;
    }

    ActivateBomb();
}

void BombHandler::EnableBombHandler()
{
    m_bIsEnabled = true;
}

void BombHandler::DisableBombHandler()
{
    m_bIsEnabled = false;
}

void BombHandler::PlaceBomb()
{
    bae::Scene* scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const auto& levelManager = LevelManager::GetInstance();

    const glm::vec2 position                = m_BombHandlerOwner->GetWorldLocation();
    const bae::Graphs::GridPosition gridPos = levelManager.GetGridComponent()->GetClosestValidNodePosition(position);

    const auto bomb = std::make_shared<bae::GameObject>("Bomb");
    bomb->SetWorldLocation(levelManager.ToPosition(gridPos));
    bomb->SetWorldScale({ LevelManager::m_GlobalScale, LevelManager::m_GlobalScale });


    const glm::vec2 hitboxDimension = levelManager.GetHitboxDimension();
    const glm::vec2 offset          = -hitboxDimension / 2.f;

    bomb->AddComponent<HitboxComponent>(*bomb, hitboxDimension, offset);
    bomb->GetComponent<HitboxComponent>()->SetVisibility(false);

    bomb->AddComponent<BombComponent>(*bomb, *m_BombHandlerOwner);

    m_PlacedBombs.push_back(bomb.get());
    scene->Add(bomb);
}

void BombHandler::ActivateBomb()
{
    std::cout << FUNCTION_NAME << '\n';
    const bae::GameObject* oldestBomb = m_PlacedBombs.front();
    oldestBomb->GetComponent<BombComponent>()->Explode();
}
