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
    Component(owner),
    Subject(owner)
{
    bae::ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 32);
    m_Owner->AddComponent<LifeComponent>(*m_Owner, 4, 3.f);

    m_Owner->AddComponent<ScoreComponent>(*m_Owner);
    m_Owner->GetComponent<ScoreComponent>()->AddObserver(this);


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

void BombermanComponent::Notify(const unsigned eventHash, Subject*, const std::any& eventData)
{
    if(GetEvent(eventHash) == Events::CollisionEvent)
    {
        HandleCollision(eventData);
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
