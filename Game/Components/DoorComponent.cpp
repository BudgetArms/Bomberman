#include "DoorComponent.hpp"

#include "Components/SpriteComponent.hpp"
#include "Core/HelperFunctions.hpp"
#include "Core/Logger.hpp"

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
        const std::string errorMessage = "EventData doesn't have data";
        bae::Logger::LogError(errorMessage);
        throw std::runtime_error(FUNCTION_NAME + errorMessage);
    }

    const auto otherHitbox = std::any_cast<HitboxComponent*>(eventData.Data);
    if(!otherHitbox)
    {
        const std::string errorMessage = "EventData doesn't have OtherHitbox";
        bae::Logger::LogError(errorMessage);
        throw std::runtime_error(FUNCTION_NAME + errorMessage);
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

