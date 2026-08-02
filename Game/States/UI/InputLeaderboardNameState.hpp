#pragma once

#include "Base/GameMode.hpp"
#include "Components/SpriteComponent.hpp"
#include "States/UI/SceneState.hpp"


namespace Game::States
{
    class InputLeaderboardNameState final : public SceneState
    {
    public:
        explicit InputLeaderboardNameState(bae::GameObject& selectionObject, GameMode gameMode, int totalScore);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<SceneState> Update() override;

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

    private:
        void UpdateSelector() const;

        static std::string GetLetter(int letterIndex);
        std::string GetLeaderboardName() const;


        std::array<bae::SpriteComponent*, 5> m_LeaderboardName{};
        bae::GameObject* m_SelectorObject{};

        bool m_bIsSelected{};

        int m_LetterIndex{};
        const int m_NrLetters{ 5 };

        GameMode m_GameMode;
        const int m_Score;
    };
}

