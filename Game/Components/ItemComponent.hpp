#pragma once

#include "Components/Component.hpp"
#include "Components/SpriteComponent.hpp"

#include "Components/HitboxComponent.hpp"
#include "Managers/LevelManager.hpp"


namespace Game
{
    class ItemComponent final : public bae::Component, public bae::Observer
    {
    public:
        explicit ItemComponent(bae::GameObject& owner, ItemType item);

        void Notify(const bae::EventData& eventData, bae::Subject* subject) override;

    private:
        void HandleCollision(const HitboxComponent& otherHitboxComponent);


        const std::string m_TexturePath{ "Textures/Items/Pickups.png" };
        const int m_SpriteNrSprites{ 3 };
        const int m_SpriteNrColumns{ 3 };

        bae::SpriteComponent* m_SpriteComponent{};
        ItemType m_ItemType;

        bool m_bHasBeenPickedUp{ false };
    };
}

