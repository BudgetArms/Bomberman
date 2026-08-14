#include "RenderCenterComponent.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include "Core/Utils.hpp"


using namespace Game;


RenderCenterComponent::RenderCenterComponent(bae::GameObject& owner) :
    Component(owner),
    m_Text{ "Default", nullptr }
{
    m_Text.m_bIsCenteredAtPosition = true;
}

void RenderCenterComponent::Render() const
{
    if(m_bShouldRenderCircle)
    {
        bae::Utils::DrawCircle(m_Owner->GetWorldLocation(), m_CircleRadius, m_CircleColor);
    }

    if(m_bShouldRenderText)
    {
        m_Text.Render();
    }
}

void RenderCenterComponent::Update()
{
    const glm::ivec2 position      = glm::ivec2(m_Owner->GetWorldLocation());
    const std::string positionText = "X: " + std::to_string(position.x) + ", Y: " + std::to_string(position.y);

    m_Text.m_Position = m_Owner->GetWorldLocation() + glm::vec2(0, -20.f);
    m_Text.SetText(positionText);
    m_Text.Update();
}

int RenderCenterComponent::GetCircleRadius() const
{
    return m_CircleRadius;
}

void RenderCenterComponent::SetCircleRadius(const int radius)
{
    if(radius <= 0)
    {
        return;
    }

    m_CircleRadius = radius;
}

bae::Utils::Color RenderCenterComponent::GetCircleColor() const
{
    return m_CircleColor;
}

void RenderCenterComponent::SetCircleColor(const bae::Utils::Color& color)
{
    m_CircleColor = color;
}

float RenderCenterComponent::GetFontSize() const
{
    return TTF_GetFontSize(m_Text.GetFont()->GetFont());
}

void RenderCenterComponent::SetFontSize(const float fontSize) const
{
    if(fontSize <= 0.f)
    {
        return;
    }

    TTF_SetFontSize(m_Text.GetFont()->GetFont(), fontSize);
}

