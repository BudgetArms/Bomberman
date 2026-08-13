#include "ItemComponent.hpp"

#include "Base/Events.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Managers/LevelManager.hpp"


using namespace Game;


ItemComponent::ItemComponent(bae::GameObject& owner, const ItemType item) :
    Component(owner),
    m_ItemType{ item }
{
    m_Owner->AddComponent<bae::SpriteComponent>(*m_Owner, m_TexturePath, SDL_FRect(0, 0, 48, 16),
                                                m_SpriteNrColumns, m_SpriteNrSprites);

    m_SpriteComponent          = m_Owner->GetComponent<bae::SpriteComponent>();
    m_SpriteComponent->m_Index = static_cast<int>(m_ItemType);
}

void ItemComponent::Notify(const unsigned eventHash, bae::Subject*, const std::any& eventData)
{
    if(m_bHasBeenPickedUp)
    {
        return;
    }

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

void ItemComponent::HandleCollision(HitboxComponent& otherHitboxComponent)
{
    const auto otherGameObject = otherHitboxComponent.GetGameObject();
    LevelManager& levelManager = LevelManager::GetInstance();

    std::vector<std::pair<bae::GameObject*, PlayerType>> playersData = levelManager.GetPlayers();
    for(const auto playerObject : playersData | std::views::keys)
    {
        if(!playerObject)
        {
            throw std::runtime_error(FUNCTION_NAME + std::string(" Failed! Invalid Player!"));
        }

        if(playerObject == otherGameObject)
        {
            std::cout << FUNCTION_NAME << " YESSS" << '\n';
            m_bHasBeenPickedUp                                                   = true;
            GetOwner()->GetComponent<HitboxComponent>()->m_bAreCollisionsEnabled = false;
            GetOwner()->Destroy();
        }
    }
}
