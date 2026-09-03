#include "DoorComponent.hpp"

#include "Components/SpriteComponent.hpp"
#include "Core/HelperFunctions.hpp"

#include "Base/Events.hpp"
#include "Components/BombermanComponent.hpp"
#include "Components/HitboxComponent.hpp"
#include "Managers/LevelManager.hpp"


using namespace Game;


DoorComponent::DoorComponent(bae::GameObject& owner) :
    Component(owner)
{
    m_Owner->AddComponent<bae::SpriteComponent>(*m_Owner, m_TexturePath, SDL_FRect(0, 0, 16, 16), 1, 1);
    m_SpriteComponent = m_Owner->GetComponent<bae::SpriteComponent>();
}

void DoorComponent::Notify(const bae::EventData& eventData, bae::Subject*)
{
    if(m_bHasOpenedDoor)
    {
        return;
    }

    const Events event = GetEvent(eventData.Hash);
    if(event != Events::CollisionEvent)
    {
        return;
    }

    if(!eventData.Data.has_value())
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to Get EventData"));
    }

    const auto otherHitbox = std::any_cast<HitboxComponent*>(eventData.Data);
    if(!otherHitbox)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed! Invalid EventData GameObject!"));
    }

    HandleCollision(*otherHitbox);
}

void DoorComponent::HandleCollision(const HitboxComponent& otherHitboxComponent)
{
    const auto otherGameObject = otherHitboxComponent.GetGameObject();
    LevelManager& levelManager = LevelManager::GetInstance();

    if(!otherGameObject->HasComponent<BombermanComponent>())
    {
        return;
    }

    if(!levelManager.GetEnemies().empty())
    {
        return;
    }

    m_bHasOpenedDoor = true;
    levelManager.SkipLevel();
}

