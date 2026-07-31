#pragma once
#include "EntityState.hpp"
#include "Core/Observer.hpp"


namespace Game::States
{
    class BombermanAliveState final : public EntityState, public bae::Observer
    {
    public:
        explicit BombermanAliveState(bae::GameObject& owner);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<EntityState> Update() override;

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

    private:
        bool m_bIsDying{ false };
    };

    class BombermanDyingState final : public EntityState
    {
    public:
        explicit BombermanDyingState(bae::GameObject& owner);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<EntityState> Update() override;

    private:
        float m_AccumulatedTime{};
        static constexpr float m_DeathDelay{ 4.f };
    };

    class BombermanDeadState final : public EntityState
    {
    public:
        explicit BombermanDeadState(bae::GameObject& owner);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<EntityState> Update();
    };
}

