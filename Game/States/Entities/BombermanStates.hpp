#pragma once
#include "EntityState.hpp"


namespace Game::States
{
    class BombermanAliveState final : public EntityState
    {
    public:
        explicit BombermanAliveState(bae::GameObject& owner);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<EntityState> Update() override;
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
}
