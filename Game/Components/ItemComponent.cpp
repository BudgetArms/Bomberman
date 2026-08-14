#include "ItemComponent.hpp"

#include "Components/SpriteComponent.hpp"
#include "Core/ServiceLocator.hpp"

#include "Base/Events.hpp"
#include "Base/SoundAssets.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/ScoreComponent.hpp"
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

    if(GetEvent(eventHash) != Events::CollisionEvent)
    {
        return;
    }

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

void ItemComponent::HandleCollision(const HitboxComponent& otherHitboxComponent)
{
    const auto otherGameObject = otherHitboxComponent.GetGameObject();
    LevelManager& levelManager = LevelManager::GetInstance();

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

    const bae::GameObject* playerObject = GetPlayerObject();
    if(!playerObject)
    {
        return;
    }

    m_bHasBeenPickedUp                                                   = true;
    GetOwner()->GetComponent<HitboxComponent>()->m_bAreCollisionsEnabled = false;

    switch(m_ItemType)
    {
        case ItemType::Bomb:
            LevelManager::GetInstance().IncreaseNrBombsAllowedContinuously();
            break;
        case ItemType::Fire:
            LevelManager::GetInstance().IncreaseBombFireRange();
            break;
        case ItemType::RemoteControl:
            LevelManager::GetInstance().EnabledRemoteControl();
            break;
    }

    // Play Powerup Sound
    bae::SoundSystem& soundSystem = bae::ServiceLocator::GetSoundSystem();
    const bae::SoundID soundId    = Game::Sounds::GetSoundId(Sounds::SoundAssets::PowerUp);
    soundSystem.Play(soundId);

    const auto scoreMap = levelManager.GetScoreMap();
    const int itemScore = scoreMap.at(ScoreType::Pickup);
    otherGameObject->GetComponent<ScoreComponent>()->AddScore(itemScore);

    GetOwner()->Destroy();
}
