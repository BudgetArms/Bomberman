#pragma once

#include "Core/Observer.hpp"

#include "Base/EnemyMovement.hpp"
#include "Base/Level.hpp"
#include "Components/HitboxComponent.hpp"
#include "States/Entities/EntityState.hpp"


namespace Game::States
{
    class EnemyAliveState final : public EntityState, public bae::Observer
    {
    public:
        explicit EnemyAliveState(bae::GameObject& owner, EnemyType enemyType);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<EntityState> Update() override;

        void Notify(const bae::EventData& eventData, bae::Subject* subject) override;

    private:
        static void HandleCollision(const HitboxComponent& otherHitboxComponent);


        std::unique_ptr<EnemyMovement> m_EnemyMovement{};
        bool m_bIsDying{ false };
    };

    class EnemyDyingState final : public EntityState
    {
    public:
        explicit EnemyDyingState(bae::GameObject& owner);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<EntityState> Update() override;

    private:
        float m_AccumulatedTime{};
        static constexpr float m_DeathDelay{ 2.f };
    };

    class EnemyDeadState final : public EntityState
    {
    public:
        explicit EnemyDeadState(bae::GameObject& owner);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<EntityState> Update() override;
    };
}

