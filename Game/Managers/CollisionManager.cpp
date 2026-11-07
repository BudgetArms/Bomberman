#include "CollisionManager.hpp"


#include "Components/HitboxComponent.hpp"


using namespace Game;


void CollisionManager::Destroy()
{
    m_HitboxObjects = {};
}

void CollisionManager::HandleCollisions() const
{
    if(m_HitboxObjects.size() < 2)
    {
        return;
    }

    for(size_t i = 0; i < m_HitboxObjects.size(); ++i)
    {
        if(!bae::GameObject::IsValid(m_HitboxObjects[i]))
        {
            continue;
        }

        auto* hitboxComponentA = m_HitboxObjects[i]->GetComponent<HitboxComponent>();
        if(!hitboxComponentA || !hitboxComponentA->m_bAreCollisionsEnabled)
        {
            continue;
        }

        for(size_t j = i + 1; j < m_HitboxObjects.size(); ++j)
        {
            if(!bae::GameObject::IsValid(m_HitboxObjects[j]))
            {
                continue;
            }

            auto* hitboxComponentB = m_HitboxObjects[j]->GetComponent<HitboxComponent>();
            if(!hitboxComponentB || !hitboxComponentB->m_bAreCollisionsEnabled)
            {
                continue;
            }


            const SDL_FRect hitboxA = hitboxComponentA->GetHitbox();
            const SDL_FRect hitboxB = hitboxComponentB->GetHitbox();
            if(SDL_HasRectIntersectionFloat(&hitboxA, &hitboxB))
            {
                SendHitboxesNotifications(hitboxComponentA, hitboxComponentB);
            }
        }
    }
}

void CollisionManager::RegisterHitbox(const HitboxComponent& hitbox)
{
    bae::GameObject* hitboxObject = hitbox.GetGameObject();
    if(!hitboxObject)
    {
        return;
    }

    const auto hitboxObjectIt = std::ranges::find_if(m_HitboxObjects, [&](const bae::GameObject* existingHitboxObject)
    {
        return hitboxObject == existingHitboxObject;
    });

    if(hitboxObjectIt != m_HitboxObjects.end())
    {
        return;
    }

    m_HitboxObjects.push_back(hitboxObject);
}

void CollisionManager::UnRegisterHitbox(const HitboxComponent* hitbox)
{
    bae::GameObject* hitboxObject = hitbox->GetGameObject();
    if(!hitboxObject)
    {
        return;
    }

    const auto hitboxObjectIt = std::ranges::find_if(m_HitboxObjects, [&](const bae::GameObject* existingHitboxObject)
    {
        return hitboxObject == existingHitboxObject;
    });

    if(hitboxObjectIt == m_HitboxObjects.end())
    {
        return;
    }

    std::erase(m_HitboxObjects, hitboxObject);
}


void CollisionManager::SendHitboxesNotifications(HitboxComponent* hitboxA, HitboxComponent* hitboxB)
{
    hitboxA->SendCollisionEventToObservers(*hitboxB);
    hitboxB->SendCollisionEventToObservers(*hitboxA);
}
