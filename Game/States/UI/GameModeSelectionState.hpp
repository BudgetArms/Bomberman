#pragma once

#include "States/UI/SceneState.hpp"


namespace Game::States
{
    class GameModeSelectionState final : public SceneState, public bae::Observer
    {
    public:
        explicit GameModeSelectionState(bae::GameObject& selectionObject);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<SceneState> Update() override;

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

    private:
        enum class GameModeSelection
        {
            SinglePlayer,
            CoOp,
            Versus,
            Back
        };

        GameModeSelection m_Selection{ GameModeSelection::SinglePlayer };
        bool m_bIsSelected{};
    };
}

