#pragma once

#include "States/UI/SceneState.hpp"


namespace Game::States
{
    class InputLeaderboardNameState final : public SceneState, public bae::Observer
    {
    public:
        explicit InputLeaderboardNameState(bae::GameObject& selectionObject);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<SceneState> Update() override;

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

    private:
        static std::string GetLetter(int letterIndex);
        std::string GetLeaderboardName() const;

        std::array<std::unique_ptr<bae::SpriteSheet>, 5> m_LeaderboardName{};


        bool m_bIsSelected{};

        int m_LetterIndex{};
        const int m_NrLetters{ 5 };
    };
}

