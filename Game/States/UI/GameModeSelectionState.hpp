#pragma once

#include "States/UI/SceneState.hpp"


namespace Game::States
{
    class GameModeSelectionState final : public SceneState
    {
    public:
        explicit GameModeSelectionState(bae::GameObject& selectionObject);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<SceneState> Update() override;

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

    private:
        void UpdateSelectorPosition();

        enum class GameModeSelection
        {
            SinglePlayer,
            CoOp,
            Versus,
            Back
        };

        GameModeSelection m_Selection{ GameModeSelection::SinglePlayer };
        bool m_bIsSelected{};

        static constexpr float m_SelectorXPosition{ 330.f };
        static constexpr float m_SelectorYOffset{ -15.f };

        std::array<glm::vec2, 4> m_SelectorPositions{};
        bae::GameObject* m_SelectorObject{};
    };
}

