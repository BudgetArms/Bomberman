#include "BombermanStates.hpp"

#include "Core/ServiceLocator.hpp"
#include "Singletons/GameTime.hpp"
#include "Sounds/SoundSystem.hpp"

#include "Base/Events.hpp"
#include "Base/SoundAssets.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/LifeComponent.hpp"
#include "Components/MovementGridComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Managers/LevelManager.hpp"


using namespace Game::States;


BombermanAliveState::BombermanAliveState(bae::GameObject& owner) :
    EntityState(owner),
    BombHandler(owner)
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
    UpdateBombs();

    if(m_bIsDying)
    {
        return std::make_unique<BombermanDyingState>(*m_Owner);
    }

    return nullptr;
}

void BombermanAliveState::Notify(const unsigned eventHash, bae::Subject*, const std::any&)
{
    if(GetEvent(eventHash) == Events::LifeLost)
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
    // Set Dying Sprite
    auto* spriteComp    = m_Owner->GetComponent<bae::SpriteComponent>();
    spriteComp->m_Index = 1;

    // Disable Hitbox
    auto* hitboxComp                    = m_Owner->GetComponent<HitboxComponent>();
    hitboxComp->m_bAreCollisionsEnabled = false;

    // Disable Movement
    const auto movementComponent = m_Owner->GetComponent<MovementGridComponent>();
    if(movementComponent)
    {
        movementComponent->Disable();
    }

    // Play Dying Sound
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



