#include "LifeComponent.hpp"

#include "Core/EventQueue.hpp"
#include "Core/HelperFunctions.hpp"
#include "Singletons/GameTime.hpp"


using namespace Game;


LifeComponent::LifeComponent(bae::GameObject& owner, const int maxLives, const float invincibilityDuration) :
    Component(owner),
    Subject(owner),
    m_Lives{ maxLives },
    m_MaxLives{ maxLives },
    m_InvincibilityDuration{ invincibilityDuration }
{
    if(m_MaxLives <= 0)
    {
        std::cout << FUNCTION_NAME << " Health is less than 0" << '\n';

        m_Lives    = 0;
        m_MaxLives = 0;
        m_bIsAlive = false;
    }
}

void LifeComponent::Update()
{
    if(IsOnDamageCooldown())
    {
        m_ElapsedDamageCooldownTime += bae::GameTime::GetInstance().GetDeltaTime();
    }


    if(!m_bIsInvincible)
    {
        return;
    }

    m_ElapsedInvincibilityTime += bae::GameTime::GetInstance().GetDeltaTime();
    if(m_ElapsedInvincibilityTime >= m_InvincibilityDuration)
    {
        SetInvincibility(false);
    }
}

void LifeComponent::AddLife()
{
    if(!IsAlive())
    {
        return;
    }

    ++m_Lives;
    SendEventToObservers(Events::LivesChanged);
}

void LifeComponent::RemoveLife()
{
    if(!IsAlive() || IsInvincible() || IsOnDamageCooldown())
    {
        return;
    }

    --m_Lives;
    m_ElapsedDamageCooldownTime = 0.0f;

    if(m_Lives > 0)
    {
        SendEventToObservers(Events::LivesChanged);
        return;
    }

    m_bIsAlive = false;
    SendEventToObservers(Events::LivesChanged);
    SendEventToObservers(Events::PlayerDied);
}

void LifeComponent::RemoveAllLives()
{
    if(!IsAlive())
    {
        return;
    }

    m_Lives    = 0;
    m_bIsAlive = false;
    SendEventToObservers(Events::LivesChanged);
    SendEventToObservers(Events::PlayerDied);
}

int LifeComponent::GetLives() const
{
    return m_Lives;
}

void LifeComponent::SetLives(const int lives)
{
    if(!IsAlive())
    {
        return;
    }
    if(lives < 0)
    {
        std::cout << FUNCTION_NAME << " Failed! lives is less than 0" << '\n';
        return;
    }

    m_Lives = lives;
    SendEventToObservers(Events::LivesChanged);
}

int LifeComponent::GetMaxLives() const
{
    return m_MaxLives;
}

void LifeComponent::SetMaxLives(const int maxLives)
{
    if(!IsAlive())
    {
        return;
    }
    if(maxLives <= 0)
    {
        std::cout << FUNCTION_NAME << " Failed! MaxLives is equal or less than 0" << '\n';
        return;
    }

    m_MaxLives = maxLives;
    SendEventToObservers(Events::LivesChanged);
}

bool LifeComponent::IsInvincible() const
{
    return m_bIsInvincible;
}

void LifeComponent::SetInvincibility(const bool isInvincible)
{
    if(!IsAlive())
    {
        return;
    }

    m_bIsInvincible            = isInvincible;
    m_ElapsedInvincibilityTime = 0;

    // SendEventToObservers(Events::InvincibilityChanged);
}

bool LifeComponent::IsAlive() const
{
    return m_bIsAlive;
}

void LifeComponent::SendEventToObservers(const Events event)
{
    NotifyObservers(GetEventHash(event));
    bae::EventQueue::GetInstance().SendEvent(GetEventHash(event));
}

bool LifeComponent::IsOnDamageCooldown() const
{
    return m_ElapsedDamageCooldownTime < m_DamageCooldownDuration;
}

