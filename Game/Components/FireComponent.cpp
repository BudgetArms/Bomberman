#include "FireComponent.hpp"

#include "Core/Subject.hpp"
#include "Components/SpriteComponent.hpp"

#include "Base/Events.hpp"
#include "Components/LifeComponent.hpp"
#include "Managers/LevelManager.hpp"
#include "Singletons/GameTime.hpp"


using namespace Game;


FireComponent::FireComponent(bae::GameObject& owner) :
    Component(owner)
{
    m_Owner->AddComponent<bae::SpriteComponent>(*m_Owner, m_FireCenterTexturePath, SDL_FRect(0, 0, 32, 16),
                                                m_SpriteNrColumns, m_SpriteNrSprites);

    m_SpriteComponent = m_Owner->GetComponent<bae::SpriteComponent>();
}

void FireComponent::Update()
{
    m_ElapsedTime += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_ElapsedTime >= m_FireDuration)
    {
        const auto hitboxComp               = m_Owner->GetComponent<HitboxComponent>();
        hitboxComp->m_bAreCollisionsEnabled = false;

        GetOwner()->Destroy();
    }
}

void FireComponent::Notify(const unsigned eventHash, bae::Subject*, const std::any& eventData)
{
    if(GetEvent(eventHash) == Events::CollisionEvent)
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

        HandleCollision(*otherHitbox);
    }
}

void FireComponent::HandleCollision(const HitboxComponent& otherHitboxComponent)
{
    LevelManager& levelManager             = LevelManager::GetInstance();
    const bae::GameObject* otherGameObject = otherHitboxComponent.GetGameObject();

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

    bae::GameObject* playerObject = GetPlayerObject();
    if(!playerObject)
    {
        return;
    }

    std::cout << FUNCTION_NAME << " Fire" << '\n';
    playerObject->GetComponent<LifeComponent>()->RemoveLife();
}
