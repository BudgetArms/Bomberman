#include "HitboxComponent.hpp"

#include "Core/GameObject.hpp"
#include "Core/Utils.hpp"

#include "Base/Events.hpp"
#include "Managers/CollisionManager.hpp"


using namespace Game;


HitboxComponent::HitboxComponent(bae::GameObject& owner, const glm::vec2& dimensions, const glm::vec2& offsetPosition) :
    Component(owner),
    Subject(owner),
    m_Dimensions{ dimensions },
    m_OffsetPosition{ offsetPosition }
{
    RegisterHitboxToCollisionManager();
}

HitboxComponent::~HitboxComponent()
{
    UnRegisterHitboxToCollisionManager();
}


void HitboxComponent::Render() const
{
    if(!m_bIsVisible)
    {
        return;
    }

    bae::Utils::DrawRect(GetHitbox(), m_Color);
}

SDL_FRect HitboxComponent::GetHitbox() const
{
    const glm::vec2 gameObjectPos = m_Owner->GetWorldLocation();
    const glm::vec2 hitboxPos     = gameObjectPos + m_OffsetPosition;

    return SDL_FRect
    {
        .x = hitboxPos.x,
        .y = hitboxPos.y,
        .w = m_Dimensions.x,
        .h = m_Dimensions.y,
    };
}

void HitboxComponent::SetDimensions(const glm::vec2& dimensions)
{
    m_Dimensions = dimensions;
}

void HitboxComponent::SetOffset(const glm::vec2& offsetPosition)
{
    m_OffsetPosition = offsetPosition;
}

void HitboxComponent::SetColor(const bae::Color& color)
{
    m_Color = color;
}

void HitboxComponent::SetVisibility(const bool visibility)
{
    m_bIsVisible = visibility;
}

void HitboxComponent::SendCollisionEventToObservers(HitboxComponent& otherHitbox)
{
    if(!m_bAreCollisionsEnabled || !otherHitbox.m_bAreCollisionsEnabled)
    {
        return;
    }

    const bae::EventData eventData
    {
        GetEventHash(Events::CollisionEvent),
        &otherHitbox
    };
    NotifyObservers(eventData);
}

void HitboxComponent::RegisterHitboxToCollisionManager() const
{
    CollisionManager::GetInstance().RegisterHitbox(*this);
}

void HitboxComponent::UnRegisterHitboxToCollisionManager() const
{
    CollisionManager::GetInstance().UnRegisterHitbox(this);
}
