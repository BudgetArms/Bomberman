#include "HitboxComponent.hpp"

#include "Managers/CollisionManager.hpp"
#include "Core/GameObject.hpp"
#include "Core/Utils.hpp"

#include "Base/Events.hpp"
#include "Managers/SceneManager.hpp"


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

    const SDL_FRect hitboxRect
    {
        hitboxPos.x,
        hitboxPos.y,
        m_Dimensions.x,
        m_Dimensions.y,
    };

    return hitboxRect;
}

void HitboxComponent::SetDimensions(const glm::vec2& dimensions)
{
    m_Dimensions = dimensions;
}

void HitboxComponent::SetOffset(const glm::vec2& offsetPosition)
{
    m_OffsetPosition = offsetPosition;
}

void HitboxComponent::SetColor(const bae::Utils::Color& color)
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

    const unsigned int eventHash = GetEventHash(Events::CollisionEvent);
    NotifyObservers(eventHash, &otherHitbox);
}

void HitboxComponent::RegisterHitboxToCollisionManager() const
{
    CollisionManager::GetInstance().RegisterHitbox(*this);
}

void HitboxComponent::UnRegisterHitboxToCollisionManager()
{
    CollisionManager::GetInstance().UnRegisterHitbox(this);
}
