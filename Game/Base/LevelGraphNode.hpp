#pragma once

#include "Graphs/Node.hpp"

#include "Base/LevelNodeType.hpp"


namespace Game
{
    class LevelGraphNode : public bae::Graphs::Node
    {
    public:
        explicit LevelGraphNode(const glm::vec2& position);

        LevelNodeType m_GridType{ LevelNodeType::Nothing };
    };
}
