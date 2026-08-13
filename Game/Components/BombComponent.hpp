#pragma once

#include "Components/Component.hpp"
#include "Components/SpriteComponent.hpp"


namespace Game
{
    class BombComponent final : public bae::Component
    {
    public:
        explicit BombComponent(bae::GameObject& owner);

        void Update() override;

    private:
        void SpawnFire();


        const std::string m_TexturePath{ "Textures/Items/Bomb.png" };
        const int m_SpriteNrSprites{ 3 };
        const int m_SpriteNrColumns{ 3 };

        bae::SpriteComponent* m_SpriteComponent{};

        bool m_bHasExploded{};
        const float m_TimeToExplode{ 2.f };
        const float m_SpriteChangeInterval{ 0.5f };
        float m_ElapsedTime{};
    };
}
