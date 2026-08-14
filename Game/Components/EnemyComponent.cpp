#include "EnemyComponent.hpp"

#include "Base/Events.hpp"
#include "Components/LifeComponent.hpp"
#include "Managers/LevelManager.hpp"
#include "States/Entities/EnemyStates.hpp"


using namespace Game;


EnemyComponent::EnemyComponent(bae::GameObject& owner, const EnemyType enemyType) :
    Component(owner),
    Subject(owner),
    m_EnemyType{ enemyType }
{
    // Life
    m_Owner->AddComponent<LifeComponent>(*m_Owner, 1, 3.f);

    // Hitbox
    m_Owner->AddComponent<HitboxComponent>(*m_Owner, glm::vec2{}, glm::vec2{});

    // Movement
    m_Owner->AddComponent<MovementGridComponent>(*m_Owner);

    m_State = std::make_unique<States::EnemyAliveState>(*m_Owner);
    m_State->OnEnter();
}

void EnemyComponent::FixedUpdate()
{
    const std::unique_ptr<States::EntityState> newState = m_State->Update();
    if(dynamic_cast<States::EnemyDeadState*>(newState.get()))
    {
        switch(m_EnemyType)
        {
            case EnemyType::Balloom:
                NotifyObservers(GetEventHash(Events::BalloomDied), GetGameObject());
                break;
            case EnemyType::Oneal:
                NotifyObservers(GetEventHash(Events::OnealDied), GetGameObject());
                break;
            case EnemyType::Doll:
                NotifyObservers(GetEventHash(Events::DollDied), GetGameObject());
                break;
            case EnemyType::Minvo:
                NotifyObservers(GetEventHash(Events::MinvoDied), GetGameObject());
                break;
            case EnemyType::BalloomPlayer:
                NotifyObservers(GetEventHash(Events::BalloomPlayerDied), GetGameObject());
                break;
        };
    }

    UpdateToNewState(m_State->Update());
}


void EnemyComponent::SetSpriteAndGridComponent()
{
    m_SpriteComponent       = m_Owner->GetComponent<bae::SpriteComponent>();
    m_MovementGridComponent = m_Owner->GetComponent<MovementGridComponent>();
}


EnemyType EnemyComponent::GetType() const
{
    return m_EnemyType;
}

int EnemyComponent::GetScore() const
{
    const bool bIsEnemyAlive = static_cast<States::EnemyAliveState*>(m_State.get());
    if(!bIsEnemyAlive)
    {
        return 0;
    }

    std::unordered_map<ScoreType, int> scoreMap = LevelManager::GetInstance().GetScoreMap();
    switch(m_EnemyType)
    {
        case EnemyType::Balloom:
            return scoreMap[ScoreType::BalloomKill];
        case EnemyType::Oneal:
            return scoreMap[ScoreType::OnealKill];
        case EnemyType::Doll:
            return scoreMap[ScoreType::DollKill];
        case EnemyType::Minvo:
            return scoreMap[ScoreType::MinvoKill];
        case EnemyType::BalloomPlayer:
            return scoreMap[ScoreType::BalloomKill];
    }

    throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to Get Score"));
}

void EnemyComponent::UpdateToNewState(std::unique_ptr<States::EntityState> newState)
{
    if(!newState)
    {
        return;
    }

    m_State->OnExit();

    m_State = std::move(newState);
    m_State->OnEnter();
}

