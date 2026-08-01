#pragma once

#include "States/UI/SceneState.hpp"


namespace Game::States
{
    class MainMenuState final : public SceneState, public bae::Observer
    {
    public:
        explicit MainMenuState(bae::GameObject& selectionObject);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<SceneState> Update() override;

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

    private:
        enum class StartMenuSelections
        {
            Play,
            Leaderboard,
            Quit
        };

        StartMenuSelections m_Selection{ StartMenuSelections::Play };
        bool m_bIsSelected{};
    };
}

