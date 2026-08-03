#pragma once

#include "Graphs/Node.hpp"

#include "Base/LevelGridType.hpp"


namespace Game
{
    class LevelGraphNode : public bae::Graphs::Node
    {
    public:
        explicit LevelGraphNode(const glm::vec2& position);

        LevelGridType m_GridType{ LevelGridType::Nothing };
    };
}
