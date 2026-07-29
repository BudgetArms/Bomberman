#pragma once

#include <glm/glm.hpp>

#include "Components/Component.hpp"
#include "Core/Observer.hpp"


namespace bae
{
    class Texture2D;
}

namespace Game
{
    class LifeDisplayComponent : public bae::Component, public bae::Observer
    {
    public:
        LifeDisplayComponent(bae::GameObject& owner, const glm::vec2& position,
                             std::shared_ptr<bae::Texture2D> lifeTexture);

        void Render() const override;


        void Notify(unsigned eventHash, bae::Subject* subject, const std::any&) override;

        void SetTexture(const std::shared_ptr<bae::Texture2D>& lifeTexture);


        glm::vec2 m_Position;
        glm::vec2 m_Margin{};
        glm::vec2 m_Scale{ 1.f, 1.f };

    private:
        std::shared_ptr<bae::Texture2D> m_LifeTexture;
        int m_Lives{};
    };
}
