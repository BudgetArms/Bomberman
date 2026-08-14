#pragma once


#include "Components/Component.hpp"
#include "Graphs/GridGraph.hpp"

#include "Components/SpriteComponent.hpp"

// Notes:
// I previously wrote the node clearing in the temporary wall component,
// but then I had to fix LevelManager::RestartLevel only getting called after the components were destroy,
// and the code looked a bit messy, due to the flag, so I removed it

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

