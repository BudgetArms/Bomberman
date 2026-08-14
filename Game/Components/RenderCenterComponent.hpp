#pragma once

#include "Components/Component.hpp"
#include "Core/Text2D.hpp"
#include "Core/Utils.hpp"


namespace Game
{
    class RenderCenterComponent : public bae::Component
    {
    public:
        explicit RenderCenterComponent(bae::GameObject& owner);

        void Render() const override;
        void Update() override;


        [[nodiscard]] int GetCircleRadius() const;
        void SetCircleRadius(int radius);

        [[nodiscard]] bae::Utils::Color GetCircleColor() const;
        void SetCircleColor(const bae::Utils::Color& color);

        [[nodiscard]] float GetFontSize() const;
        void SetFontSize(float fontSize) const;


        bool m_bShouldRenderCircle{ true };
        bool m_bShouldRenderText{ false };

    private:
        int m_CircleRadius{ 5 };

        bae::Utils::Color m_CircleColor{ bae::Utils::Color::Cyan };

        bae::Text2D m_Text;
    };
}

