#include "ScoreDisplayComponent.hpp"

#include "Core/HelperFunctions.hpp"
#include "Core/Text2D.hpp"

#include "Base/Events.hpp"
#include "Components/ScoreComponent.hpp"


using namespace Game;


ScoreDisplayComponent::ScoreDisplayComponent(bae::GameObject& owner, const glm::vec2& position,
                                             std::unique_ptr<bae::Text2D> text) :
    Component(owner),
    m_Text{ std::move(text) }
{
    SetPosition(position);
}

void ScoreDisplayComponent::Update()
{
    m_Text->Update();
}

void ScoreDisplayComponent::Render() const
{
    m_Text->Render();
}

bae::Text2D* ScoreDisplayComponent::GetText() const
{
    return m_Text.get();
}

glm::vec2 ScoreDisplayComponent::GetPosition() const
{
    return m_Text->m_Position;
}

void ScoreDisplayComponent::SetPosition(const glm::vec2& position) const
{
    m_Text->m_Position = position;
}

std::string ScoreDisplayComponent::GetPreScoreText() const
{
    return m_PreScoreText;
}

void ScoreDisplayComponent::SetPreScoreText(const std::string& text)
{
    m_PreScoreText = text;
    UpdateDisplayText();
}

void ScoreDisplayComponent::Notify(const unsigned eventHash, bae::Subject*, const std::any&)
{
    if(GetEvent(eventHash) == Events::ScoreChanged)
    {
        UpdateDisplayText();
    }
}

void ScoreDisplayComponent::UpdateDisplayText() const
{
    const ScoreComponent* scoreComponent = m_Owner->GetComponent<ScoreComponent>();
    m_Text->SetText(m_PreScoreText + std::to_string(scoreComponent->GetScore()));
}
