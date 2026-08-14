#pragma once

#include <array>

#include "Components/Component.hpp"
#include "Components/SpriteComponent.hpp"
#include "Core/Observer.hpp"

#include "Base/DirectionEnum.hpp"
#include "Components/HitboxComponent.hpp"
#include "Graphs/GridGraph.hpp"


namespace Game
{
    class FireComponent final : public bae::Component, public bae::Observer
    {
    public:
        explicit FireComponent(bae::GameObject& owner, bae::GameObject& instigatorObject);

        void Update() override;
        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

    private:
        static void HandleCollision(const HitboxComponent& otherHitboxComponent);
        static void HandleIfPlayerCollision(const HitboxComponent& otherHitboxComponent);
        static void HandleIfTemporaryWallCollision(const HitboxComponent& otherHitboxComponent);

        void AddFireChild(Direction directionFire, int childFireRange);

        void AddFireChildren();

        static Direction GetDirectionFromGridPos(const bae::Graphs::GridPosition& gridPos);


        const std::string m_FireCenterTexturePath{ "Textures/Items/FireCenter.png" };
        const std::string m_FireTexturePath{ "Textures/Items/Fire.png" };
        const int m_SpriteNrSprites{ 2 };
        const int m_SpriteNrColumns{ 2 };

        bae::SpriteComponent* m_SpriteComponent{};
        bae::GameObject* m_Instigator;

        std::unordered_map<Direction, bae::GameObject*> m_FireObjects{};

        // Todo: this should be gotten from level manager but time constraint
        const float m_GridSize{ 32.f };
        const glm::vec2 m_HitboxDimension{ 30.f, 30.f };
        const float m_FireDuration{ 1.f };
        float m_ElapsedTime{};


        static constexpr std::array<bae::Graphs::GridPosition, 4> m_Directions
        {
            bae::Graphs::GridPosition{ 1, 0 },
            bae::Graphs::GridPosition{ -1, 0 },
            bae::Graphs::GridPosition{ 0, 1 },
            bae::Graphs::GridPosition{ 0, -1 }
        };
    };
}
