#include "BombermanStates.hpp"

#include "Components/HitboxComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Core/ServiceLocator.hpp"
#include "Singletons/GameTime.hpp"
#include "Sounds/SoundSystem.hpp"


using namespace Game::States;


BombermanAliveState::BombermanAliveState(bae::GameObject& owner) :
    EntityState(owner)
{
}

void BombermanAliveState::OnEnter()
{
    auto* spriteComp    = m_Owner->GetComponent<bae::SpriteComponent>();
    spriteComp->m_Index = 0;
}

void BombermanAliveState::OnExit()
{
}

std::unique_ptr<EntityState> BombermanAliveState::Update()
{
    return nullptr;
}


BombermanDyingState::BombermanDyingState(bae::GameObject& owner) :
    EntityState(owner)
{
}

void BombermanDyingState::OnEnter()
{
    auto* spriteComp    = m_Owner->GetComponent<bae::SpriteComponent>();
    spriteComp->m_Index = 1;

    auto* hitboxComp                    = m_Owner->GetComponent<HitboxComponent>();
    hitboxComp->m_bAreCollisionsEnabled = false;
}

void BombermanDyingState::OnExit()
{
}

std::unique_ptr<EntityState> BombermanDyingState::Update()
{
    m_AccumulatedTime += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_AccumulatedTime >= m_DeathDelay)
    {
        m_Owner->Destroy();
    }

    return nullptr;
}



