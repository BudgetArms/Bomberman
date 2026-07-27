#pragma once

#include <vector>

#include "Components/Component.hpp"
#include "Singletons/Singleton.hpp"


namespace Game
{
    class HitboxComponent;

    class CollisionManager : public bae::Singleton<CollisionManager>
    {
    public:
        void HandleCollisions() const;

        void RegisterHitbox(const HitboxComponent& hitbox);
        void UnRegisterHitbox(HitboxComponent* hitbox);

    private:
        static void SendHitboxesNotifications(HitboxComponent* hitboxA, HitboxComponent* hitboxB);


        std::vector<bae::GameObject*> m_HitboxObjects{};
    };
}
