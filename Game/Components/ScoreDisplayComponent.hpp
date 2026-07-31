#pragma once

#include "Components/Component.hpp"
#include "Core/Observer.hpp"


namespace bae
{
    class Text2D;
}

namespace Game
{
    class ScoreComponent;

    class ScoreDisplayComponent : public bae::Component, public bae::Observer
    {
    public:
        ScoreDisplayComponent(bae::GameObject& owner, const glm::vec2& position, std::unique_ptr<bae::Text2D> text);

        void Update() override;
        void Render() const override;

        [[nodiscard]] bae::Text2D* GetText() const;

        [[nodiscard]] glm::vec2 GetPosition() const;
        void SetPosition(const glm::vec2& position) const;

        [[nodiscard]] std::string GetPreScoreText() const;
        void SetPreScoreText(const std::string& text);

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any&) override;

    private:
        void UpdateDisplayText() const;

        std::shared_ptr<bae::Text2D> m_Text;

        std::string m_PreScoreText{};
    };
}
