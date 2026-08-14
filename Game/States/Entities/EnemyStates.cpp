#include "EnemyStates.hpp"

#include <unordered_map>

#include "Components/SpriteComponent.hpp"
#include "Core/ServiceLocator.hpp"
#include "Singletons/GameTime.hpp"

#include "Base/Events.hpp"
#include "Components/EnemyComponent.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/LifeComponent.hpp"
#include "Components/MovementGridComponent.hpp"
#include "Managers/LevelManager.hpp"


using namespace Game::States;


EnemyAliveState::EnemyAliveState(bae::GameObject& owner) :
    EntityState(owner)
{
    const std::unordered_map<EnemyType, SharedEnemyInfo> sharedEnemyInfo
            = LevelManager::GetInstance().GetEnemySharedInfo();

    const EnemyType enemyType        = m_Owner->GetComponent<EnemyComponent>()->GetType();
    const SharedEnemyInfo& enemyInfo = sharedEnemyInfo.at(enemyType);

    m_EnemyMovement = std::make_unique<EnemyMovement>(owner, enemyInfo.Speed, enemyInfo.Intelligence,
                                                      enemyInfo.DirectionUpChance);
}

void EnemyAliveState::OnEnter()
{
    m_Owner->GetComponent<LifeComponent>()->AddObserver(this);
    m_Owner->GetComponent<HitboxComponent>()->AddObserver(this);
}

void EnemyAliveState::OnExit()
{
}

std::unique_ptr<EntityState> EnemyAliveState::Update()
{
    if(m_bIsDying)
    {
        return std::make_unique<EnemyDyingState>(*m_Owner);
    }

    m_EnemyMovement->Update();

    return nullptr;
}

void EnemyAliveState::Notify(const unsigned eventHash, bae::Subject*, const std::any& eventData)
{
    const Events event = GetEvent(eventHash);
    if(event == Events::LivesChanged)
    {
        m_bIsDying = true;
    }

    if(event != Events::CollisionEvent)
    {
        return;
    }


    if(!eventData.has_value())
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to Get EventData"));
    }

    const auto otherHitbox = std::any_cast<HitboxComponent*>(eventData);
    if(!otherHitbox)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed! Invalid EventData GameObject!"));
    }

    HandleCollision(*otherHitbox);
}

void EnemyAliveState::HandleCollision(const HitboxComponent& otherHitboxComponent)
{
    const auto otherGameObject = otherHitboxComponent.GetGameObject();
    LevelManager& levelManager = LevelManager::GetInstance();

    std::vector<std::pair<bae::GameObject*, PlayerType>> playersData = levelManager.GetPlayers();

    auto GetPlayerObject = [&]() -> bae::GameObject*
    {
        for(bae::GameObject* playerObject : playersData | std::views::keys)
        {
            if(playerObject == otherGameObject)
            {
                return playerObject;
            }
        }

        return nullptr;
    };

    const bae::GameObject* playerObject = GetPlayerObject();
    if(!playerObject)
    {
        return;
    }

    playerObject->GetComponent<LifeComponent>()->RemoveLife();
}


EnemyDyingState::EnemyDyingState(bae::GameObject& owner) :
    EntityState(owner)
{
}

void EnemyDyingState::OnEnter()
{
    // Set Dying Sprite
    auto* spriteComp    = m_Owner->GetComponent<bae::SpriteComponent>();
    spriteComp->m_Index = 1;

    // Disable Hitbox
    auto* hitboxComp                    = m_Owner->GetComponent<HitboxComponent>();
    hitboxComp->m_bAreCollisionsEnabled = false;

    // Disable Movement
    const auto movementComponent = m_Owner->GetComponent<MovementGridComponent>();
    if(movementComponent)
    {
        movementComponent->Disable();
    }

    // No sound effect :(
}

void EnemyDyingState::OnExit()
{
}

std::unique_ptr<EntityState> EnemyDyingState::Update()
{
    m_AccumulatedTime += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_AccumulatedTime >= m_DeathDelay)
    {
        return std::make_unique<EnemyDeadState>(*m_Owner);
    }

    return nullptr;
}


EnemyDeadState::EnemyDeadState(bae::GameObject& owner) :
    EntityState(owner)
{
}

void EnemyDeadState::OnEnter()
{
}

void EnemyDeadState::OnExit()
{
}

std::unique_ptr<EntityState> EnemyDeadState::Update()
{
    return nullptr;
}



