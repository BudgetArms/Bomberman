#include "BombermanComponent.hpp"

#include "Managers/ResourceManager.hpp"

#include "Base/Events.hpp"
#include "Components/EnemyComponent.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/LifeComponent.hpp"
#include "Components/ScoreComponent.hpp"
#include "States/Entities/BombermanStates.hpp"


using namespace Game;


BombermanComponent::BombermanComponent(bae::GameObject& owner) :
    Component(owner),
    Subject(owner)
{
    bae::ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 32);
    m_Owner->AddComponent<LifeComponent>(*m_Owner, 4, 3.f);

    m_Owner->AddComponent<ScoreComponent>(*m_Owner);

    m_State = std::make_unique<States::BombermanAliveState>(owner);
    m_State->OnEnter();
}

void BombermanComponent::Update()
{
    std::unique_ptr<States::EntityState> newState = m_State->Update();
    if(dynamic_cast<States::BombermanDeadState*>(newState.get()))
    {
        NotifyObservers(GetEventHash(Events::PlayerDied), GetGameObject());
    }

    UpdateToNewState(std::move(newState));
}


void BombermanComponent::UpdateToNewState(std::unique_ptr<States::EntityState> newState)
{
    if(!newState)
    {
        return;
    }

    m_State->OnExit();
    m_State = std::move(newState);
    m_State->OnEnter();
}


void BombermanComponent::TryPlaceBomb() const
{
    const auto bombHandler = dynamic_cast<BombHandler*>(m_State.get());
    if(!bombHandler)
    {
        return;
    }

    bombHandler->TryPlaceBomb();
}

void BombermanComponent::TryActivateBomb() const
{
    const auto bombHandler = dynamic_cast<BombHandler*>(m_State.get());
    if(!bombHandler)
    {
        return;
    }

    bombHandler->TryActivateBomb();
}
