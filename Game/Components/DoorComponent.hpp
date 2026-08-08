#pragma once

#include "Components/Component.hpp"

#include "Components/SpriteComponent.hpp"


namespace Game
{
    class DoorComponent : public bae::Component
    {
    public:
        explicit DoorComponent(bae::GameObject& owner);

    private:
        const std::string m_TexturePath{ "Textures/Level/Door.png" };
        bae::SpriteComponent* m_SpriteComponent{};
    };
}

