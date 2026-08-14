#pragma once

#include "HitboxComponent.hpp"
#include "Components/Component.hpp"
#include "Core/Observer.hpp"

#include "Components/SpriteComponent.hpp"


namespace Game
{
    class DoorComponent final : public bae::Component, public bae::Observer
    {
    public:
        explicit DoorComponent(bae::GameObject& owner);

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

    private:
        void HandleCollision(const HitboxComponent& otherHitboxComponent);


        bool m_bHasOpenedDoor{};

        const std::string m_TexturePath{ "Textures/Level/Door.png" };
        bae::SpriteComponent* m_SpriteComponent{};
    };
}

