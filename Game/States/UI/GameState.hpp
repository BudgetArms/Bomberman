#pragma once

#include "States/UI/SceneState.hpp"


namespace Game::States
{
    class GameState final : public SceneState, public bae::EventListener
    {
    public:
        explicit GameState(bae::GameObject& selectionObject);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<SceneState> Update() override;

        void HandleEvent(unsigned eventHash) override;

    private:
        bool m_bIsGameFinished{};
    };
}

