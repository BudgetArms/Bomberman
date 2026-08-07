#pragma once


#include "Components/Component.hpp"
#include "Components/SpriteComponent.hpp"
#include "Graphs/GridGraph.hpp"


namespace Game
{
    class TemporaryWallComponent final : public bae::Component
    {
    public:
        explicit TemporaryWallComponent(bae::GameObject& owner);

    private:
        const std::string m_TexturePath{ "Textures/Level/TempBlock.png" };
        bae::SpriteComponent* m_SpriteComponent{};
    };
}

