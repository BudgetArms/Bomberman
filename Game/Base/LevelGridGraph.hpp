#pragma once

#include "Graphs/GridGraph.hpp"

#include "Base/LevelGridType.hpp"


namespace Game
{
    class LevelGridGraph : public bae::Graphs::GridGraph
    {
    public:
        LevelGridGraph(const glm::vec2& position, int columns, int rows, const glm::ivec2& cellSize);

        void SetNodeTerrainType(int nodeId, LevelGridType type) const;
        void SetNodeTerrainType(const glm::vec2& position, LevelGridType type) const;
    };
}
