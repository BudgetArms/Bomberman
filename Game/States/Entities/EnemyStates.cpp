#include "EnemyStates.hpp"

#include "Components/SpriteComponent.hpp"
#include "Core/ServiceLocator.hpp"
#include "Singletons/GameTime.hpp"

#include "Base/Events.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/LifeComponent.hpp"
#include "Components/MovementGridComponent.hpp"
#include "Managers/LevelManager.hpp"


using namespace Game::States;


EnemyAliveState::EnemyAliveState(bae::GameObject& owner) :
    EntityState(owner)
{
}

void EnemyAliveState::OnEnter()
{
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

    return nullptr;
}

void EnemyAliveState::Notify(const unsigned eventHash, bae::Subject*, const std::any&)
{
    const Events event = GetEvent(eventHash);
    if(event == Events::BalloomDied ||
        event == Events::OnealDied ||
        event == Events::DollDied ||
        event == Events::MinvoDied
    )
    {
        m_bIsDying = true;
    }
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



