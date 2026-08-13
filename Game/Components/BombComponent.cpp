#include "BombComponent.hpp"

#include <glm/glm.hpp>

#include "Core/Scene.hpp"
#include "Singletons/GameTime.hpp"

#include "Base/CommonManagerVariables.hpp"
#include "Base/Events.hpp"
#include "Components/FireComponent.hpp"
#include "Components/HitboxComponent.hpp"
#include "Managers/LevelManager.hpp"


using namespace Game;

BombComponent::BombComponent(bae::GameObject& owner) :
    Component(owner),
    Subject(owner)
{
    m_Owner->AddComponent<bae::SpriteComponent>(*m_Owner, m_TexturePath, SDL_FRect(0, 0, 48, 16),
                                                m_SpriteNrColumns, m_SpriteNrSprites);

    m_SpriteComponent = m_Owner->GetComponent<bae::SpriteComponent>();
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
        m_bHasExploded = true;
        SpawnFire();

        NotifyObservers(GetEventHash(Events::BombExplosion));
        GetOwner()->Destroy();
        return;
    }


    m_SpriteChangedElapsedTime += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_SpriteChangedElapsedTime >= m_SpriteChangeInterval)
    {
        m_SpriteChangedElapsedTime = 0.f;
        m_SpriteComponent->NextSprite();
    }
}

void BombComponent::ForceExplode()
{
    m_bHasExploded = true;
    SpawnFire();

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
    const auto fireComp = fire->GetComponent<FireComponent>();

    const glm::vec2 hitboxDimension = LevelManager::GetInstance().GetHitboxDimension();
    const glm::vec2 offset          = -hitboxDimension / 2.f;

    fire->AddComponent<HitboxComponent>(*fire, hitboxDimension, offset);
    fire->GetComponent<HitboxComponent>()->SetVisibility(false);
    fire->GetComponent<HitboxComponent>()->AddObserver(fireComp);

    scene->Add(fire);
}
