#include "BombermanStates.hpp"

#include "Base/Events.hpp"
#include "Base/SoundAssets.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/LifeComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Core/EventQueue.hpp"
#include "Core/ServiceLocator.hpp"
#include "Singletons/GameTime.hpp"
#include "Sounds/SoundSystem.hpp"


using namespace Game::States;


BombermanAliveState::BombermanAliveState(bae::GameObject& owner) :
    EntityState(owner)
{
}

void BombermanAliveState::OnEnter()
{
    auto* spriteComp    = m_Owner->GetComponent<bae::SpriteComponent>();
    spriteComp->m_Index = 0;

    m_Owner->GetComponent<LifeComponent>()->AddObserver(this);
}

void BombermanAliveState::OnExit()
{
}

std::unique_ptr<EntityState> BombermanAliveState::Update()
{
    std::cout << FUNCTION_NAME << '\n';
    if(m_bIsDying)
    {
        return std::make_unique<BombermanDyingState>(*m_Owner);
    }

    return nullptr;
}

void BombermanAliveState::Notify(const unsigned eventHash, bae::Subject*, const std::any&)
{
    if(GetEvent(eventHash) == Events::LivesChanged)
    {
        m_bIsDying = true;
    }
}


BombermanDyingState::BombermanDyingState(bae::GameObject& owner) :
    EntityState(owner)
{
}

void BombermanDyingState::OnEnter()
{
    auto* spriteComp    = m_Owner->GetComponent<bae::SpriteComponent>();
    spriteComp->m_Index = 1;

    auto* hitboxComp                    = m_Owner->GetComponent<HitboxComponent>();
    hitboxComp->m_bAreCollisionsEnabled = false;


    // Play Sound
    bae::SoundSystem& soundSystem   = bae::ServiceLocator::GetSoundSystem();
    const bae::SoundID startSoundID = Game::Sounds::GetSoundId(Sounds::SoundAssets::BombermanKilled);

    const bae::ActiveSoundID playingSoundID = soundSystem.Play(startSoundID);
    soundSystem.SetVolume(playingSoundID, 1.f);
}

void BombermanDyingState::OnExit()
{
}

std::unique_ptr<EntityState> BombermanDyingState::Update()
{
    m_AccumulatedTime += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_AccumulatedTime >= m_DeathDelay)
    {
        return std::make_unique<BombermanDeadState>(*m_Owner);
    }

    return nullptr;
}


BombermanDeadState::BombermanDeadState(bae::GameObject& owner) :
    EntityState(owner)
{
}

void BombermanDeadState::OnEnter()
{
}

void BombermanDeadState::OnExit()
{
}

std::unique_ptr<EntityState> BombermanDeadState::Update()
{
    return nullptr;
}



