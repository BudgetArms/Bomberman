#include "BombermanComponent.hpp"

#include "HitboxComponent.hpp"
#include "LifeComponent.hpp"
#include "ScoreComponent.hpp"
#include "Base/Events.hpp"
#include "Components/SpriteComponent.hpp"
#include "Managers/ResourceManager.hpp"
#include "States/Entities/BombermanStates.hpp"


using namespace Game;


BombermanComponent::BombermanComponent(bae::GameObject& owner) :
    Component(owner)
{
    bae::ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 32);
    m_Owner->AddComponent<LifeComponent>(*m_Owner, 4, 3.f);
    m_Owner->GetComponent<LifeComponent>()->AddObserver(this);

    m_Owner->AddComponent<bae::SpriteComponent>(*m_Owner, "Textures/Characters/Bomberman.png",
                                                SDL_FRect(0, 0, 32, 16), 2, 1);

    m_Owner->AddComponent<ScoreComponent>(*m_Owner);
    m_Owner->GetComponent<ScoreComponent>()->AddObserver(this);


    m_State = std::make_unique<States::BombermanAliveState>(owner);
    m_State->OnEnter();
}

void BombermanComponent::Update()
{
    UpdateToNewState(m_State->Update());
}


void BombermanComponent::UpdateToNewState(std::unique_ptr<States::EntityState> newState)
{
    // if in dying state, ignore new state
    if(dynamic_cast<States::BombermanDyingState*>(m_State.get()))
    {
        return;
    }

    if(!newState)
    {
        return;
    }

    m_State->OnExit();
    m_State = std::move(newState);
    m_State->OnEnter();
}

void BombermanComponent::Notify(const unsigned eventHash, bae::Subject*, const std::any& eventData)
{
    switch(GetEvent(eventHash))
    {
        case Events::PlayerDied:
        case Events::DirectionChanged:
        case Events::GameWon:
        case Events::GameOver:
        case Events::LevelWon:
        case Events::LevelLost:
        case Events::BalloomDied:
        case Events::OnealDied:
        case Events::DollDied:
        case Events::MinvoDied:
        case Events::BeginLevel:
        case Events::RestartLevel:
        case Events::ScoreChanged:
        case Events::LivesChanged:
            break;
        case Events::CollisionEvent:
            HandleCollision(eventData);
            break;
        case Events::NoEvent:
            break;
        default:;
    }
}

void BombermanComponent::HandleCollision(const std::any& eventData) const
{
    if(!eventData.has_value())
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to Get EventData"));
    }

    const auto otherHitbox = std::any_cast<HitboxComponent*>(eventData);
    if(!otherHitbox)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed! Invalid EventData GameObject!"));
    }

    /*
    bae::GameObject* otherObject = otherHitbox->GetGameObject();
    if(const auto itemComp = otherObject->GetComponent<ItemComponent>())
    {
        HandleItemCollision(*itemComp);
    }
    else if(IsEnemy(otherObject))
    {
        HandleEnemyCollision(otherObject);
    }
    */
}


void BombermanComponent::HandleEnemyCollision(bae::GameObject*) const
{
    const auto lifeComp = m_Owner->GetComponent<LifeComponent>();
    if(!lifeComp)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to Get LifeComponent!"));
    }

    if(lifeComp->IsInvincible())
    {
        std::cout << "Invincible???????\n";
    }
    else
    {
        std::cout << "Removed Life\n";
        lifeComp->RemoveLife();
    }
}
