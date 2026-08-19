#pragma once

#include "States/UI/SceneState.hpp"


namespace Game::States
{
    class MainMenuState final : public SceneState
    {
    public:
        explicit MainMenuState(bae::GameObject& selectionObject);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<SceneState> Update() override;

        void Notify(const bae::EventData& eventData, bae::Subject* subject) override;

    private:
        void UpdateSelectorPosition();

        enum class StartMenuSelections
        {
            Play,
            Leaderboard,
            Quit
        };

        StartMenuSelections m_Selection{ StartMenuSelections::Play };
        bool m_bIsSelected{};

        static constexpr float m_SelectorXPosition{ 290.f };
        static constexpr float m_SelectorYOffset{ -15.f };

        std::array<glm::vec2, 3> m_SelectorPositions{};
        bae::GameObject* m_SelectorObject{};
    };
}

