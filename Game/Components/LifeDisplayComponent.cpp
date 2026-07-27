#include "LifeDisplayComponent.hpp"

#include "Wrappers/Texture2D.hpp"

#include "Base/Events.hpp"
#include "Components/LifeComponent.hpp"
#include "Core/Renderer.hpp"


using namespace Game;


LifeDisplayComponent::LifeDisplayComponent(bae::GameObject& owner, const glm::vec2& position,
                                           std::shared_ptr<bae::Texture2D> texture) :
    Component(owner),
    m_Position{ position },
    m_LifeTexture{ texture }
{
    const LifeComponent* lifeComponent = m_Owner->GetComponent<LifeComponent>();
    m_Lives                            = lifeComponent->GetLives();
}

void LifeDisplayComponent::Render() const
{
    for(int i{}; i < m_Lives - 1; ++i)
    {
        const glm::vec2 imageSize   = m_Scale * m_LifeTexture->GetSize();
        const glm::vec2 newPosition = m_Position + glm::vec2(i, i) * (m_Margin + glm::vec2{ imageSize.x, 0 });

        bae::Renderer::GetInstance().RenderTexture(*m_LifeTexture.get(), false, newPosition, 0.f, m_Scale);
    }
}


void LifeDisplayComponent::Notify(const unsigned eventHash, bae::Subject* subject, const std::any&)
{
    if(GetEvent(eventHash) == Events::LivesChanged)
    {
        const LifeComponent* lifeComponent = subject->GetGameObject()->GetComponent<LifeComponent>();
        m_Lives                            = lifeComponent->GetLives();
    }
}

void LifeDisplayComponent::SetTexture(const std::shared_ptr<bae::Texture2D>& lifeTexture)
{
    m_LifeTexture = lifeTexture;
}
