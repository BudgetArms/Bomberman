#include "BombComponent.hpp"

#include "Core/Scene.hpp"
#include "Singletons/GameTime.hpp"

#include "Base/CommonManagerVariables.hpp"
#include "Components/FireComponent.hpp"
#include "Components/HitboxComponent.hpp"
#include "Managers/LevelManager.hpp"


using namespace Game;

BombComponent::BombComponent(bae::GameObject& owner) :
    Component(owner)
{
}

void BombComponent::Update()
{
    if(m_bHasExploded)
    {
        return;
    }

    m_ElapsedTime += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_ElapsedTime > m_TimeToExplode)
    {
        m_bHasExploded = true;
    }
}

void BombComponent::SpawnFire()
{
    bae::Scene* const scene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());

    const glm::vec2 position = GetOwner()->GetWorldLocation();

    const auto fire = std::make_shared<bae::GameObject>("Fire");
    fire->SetWorldLocation(position);
    fire->SetWorldScale({ 2.f, 2.f });

    fire->AddComponent<FireComponent>(*fire);
    const auto fireComp = fire->GetComponent<FireComponent>();

    const glm::vec2 hitboxDimension = LevelManager::GetInstance().GetHitboxDimension();
    const glm::vec2 offset          = -hitboxDimension / 2.f;

    fire->AddComponent<HitboxComponent>(*fire, hitboxDimension, offset);
    fire->GetComponent<HitboxComponent>()->SetVisibility(false);
    fire->GetComponent<HitboxComponent>()->AddObserver(fireComp);

    scene->Add(fire);
}
