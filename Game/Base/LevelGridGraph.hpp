#pragma once

#include "Graphs/GridGraph.hpp"

#include "Base/LevelNodeType.hpp"


namespace Game
{
    class LevelGridGraph : public bae::Graphs::GridGraph
    {
    public:
        LevelGridGraph(const glm::vec2& position, int columns, int rows, const glm::ivec2& cellSize);

        [[nodiscard]] LevelNodeType GetNodeType(const bae::Graphs::GridPosition& gridPosition) const;

        void SetNodeType(const bae::Graphs::GridPosition& gridPosition, LevelNodeType type) const;
        void SetNodeType(const glm::vec2& position, LevelNodeType type) const;
    };
}
