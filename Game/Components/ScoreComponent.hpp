#pragma once

#include "Components/Component.hpp"
#include "Core/Subject.hpp"


namespace Game
{
    class ScoreComponent final : public bae::Component, public bae::Subject
    {
    public:
        explicit ScoreComponent(bae::GameObject& owner);

        // Score can be negative
        // The current score can also be negative
        void AddScore(int score);

        [[nodiscard]] int GetScore() const;

        // The score can be set negative
        void SetScore(int score);

    private:
        int m_Score{};
    };
}
