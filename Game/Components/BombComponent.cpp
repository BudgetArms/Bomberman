#include "BombComponent.hpp"

#include <glm/glm.hpp>

#include "Core/Scene.hpp"
#include "Singletons/GameTime.hpp"

#include "Base/CommonManagerVariables.hpp"
#include "Base/Events.hpp"
#include "Base/SoundAssets.hpp"
#include "Components/FireComponent.hpp"
#include "Components/HitboxComponent.hpp"
#include "Core/ServiceLocator.hpp"
#include "Managers/LevelManager.hpp"


using namespace Game;

BombComponent::BombComponent(bae::GameObject& owner) :
    Component(owner),
    Subject(owner)
{
    m_Owner->AddComponent<bae::SpriteComponent>(*m_Owner, m_TexturePath, SDL_FRect(0, 0, 48, 16),
                                                m_SpriteNrColumns, m_SpriteNrSprites);

    m_SpriteComponent = m_Owner->GetComponent<bae::SpriteComponent>();

    // Play BombLay Sound
    bae::SoundSystem& soundSystem = bae::ServiceLocator::GetSoundSystem();
    const bae::SoundID soundId    = Game::Sounds::GetSoundId(Sounds::SoundAssets::BombLay);
    soundSystem.Play(soundId);
}

void BombComponent::Update()
{
    if(m_bHasExploded)
    {
        return;
    }

    m_BombExplosionElapsedTime += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_BombExplosionElapsedTime > m_TimeToExplode)
    {
        Explode();
        return;
    }


    m_SpriteChangedElapsedTime += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_SpriteChangedElapsedTime >= m_SpriteChangeInterval)
    {
        m_SpriteChangedElapsedTime = 0.f;
        m_SpriteComponent->NextSprite();
    }
}

void BombComponent::Explode()
{
    m_bHasExploded = true;
    SpawnFire();

    // Play BombExplosion Sound
    bae::SoundSystem& soundSystem = bae::ServiceLocator::GetSoundSystem();
    const bae::SoundID soundId    = Game::Sounds::GetSoundId(Sounds::SoundAssets::BombExplosion);
    soundSystem.Play(soundId);

    NotifyObservers(GetEventHash(Events::BombExplosion));
    GetOwner()->Destroy();
}

void BombComponent::SpawnFire()
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelBackgroundName.data());

    const glm::vec2 position = GetOwner()->GetWorldLocation();

    const auto fire = std::make_shared<bae::GameObject>("Fire");
    fire->SetWorldLocation(position);
    fire->SetWorldScale({ LevelManager::m_GlobalScale, LevelManager::m_GlobalScale });

    fire->AddComponent<FireComponent>(*fire);

    scene->Add(fire);
}
