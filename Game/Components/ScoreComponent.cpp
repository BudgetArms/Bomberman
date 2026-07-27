#include "ScoreComponent.hpp"

#include "Base/Events.hpp"


using namespace Game;


ScoreComponent::ScoreComponent(bae::GameObject& owner) :
    Component(owner),
    Subject(owner)
{
}

void ScoreComponent::AddScore(const int score)
{
    m_Score                      += score;
    const unsigned int eventHash = GetEventHash(Events::ScoreChanged);
    NotifyObservers(eventHash);
}

int ScoreComponent::GetScore() const
{
    return m_Score;
}

void ScoreComponent::SetScore(const int score)
{
    m_Score                      = score;
    const unsigned int eventHash = GetEventHash(Events::ScoreChanged);
    NotifyObservers(eventHash);
}

