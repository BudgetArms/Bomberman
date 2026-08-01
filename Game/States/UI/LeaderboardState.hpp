#pragma once

#include "States/UI/SceneState.hpp"


namespace Game::States
{
    class LeaderboardState final : public SceneState, public bae::Observer
    {
    public:
        explicit LeaderboardState(bae::GameObject& selectionObject);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<SceneState> Update() override;

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

    private:
        bool m_bIsSelected{};
    };
}

