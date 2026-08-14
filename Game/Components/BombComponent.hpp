#pragma once

#include "Components/Component.hpp"
#include "Components/SpriteComponent.hpp"
#include "Core/Subject.hpp"


namespace Game
{
    class BombComponent final : public bae::Component, public bae::Subject
    {
    public:
        explicit BombComponent(bae::GameObject& owner, bae::GameObject& instigatorObject);

        void Update() override;

        void Explode();

    private:
        void SpawnFire();


        const std::string m_TexturePath{ "Textures/Items/Bomb.png" };
        const int m_SpriteNrSprites{ 3 };
        const int m_SpriteNrColumns{ 3 };

        bae::SpriteComponent* m_SpriteComponent{};
        bae::GameObject* m_Instigator;

        bool m_bHasExploded{};
        const float m_TimeToExplode{ 2.f };
        const float m_SpriteChangeInterval{ 0.5f };

        float m_BombExplosionElapsedTime{};
        float m_SpriteChangedElapsedTime{};
    };
}
