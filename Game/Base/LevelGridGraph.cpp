#include "LevelGridGraph.hpp"

#include "Core/Logger.hpp"

#include "Base/LevelGraphNode.hpp"


Game::LevelGridGraph::LevelGridGraph(const glm::vec2& position, const int columns, const int rows,
                                     const glm::ivec2& cellSize) :
    GridGraph(position, columns, rows, cellSize, false, false,
              std::make_unique<bae::Graphs::NodeFactoryTemplate<LevelGraphNode>>())
{
}

Game::LevelNodeType Game::LevelGridGraph::GetNodeType(const bae::Graphs::GridPosition& gridPosition) const
{
    const auto node      = GetNode(gridPosition);
    const auto levelNode = dynamic_cast<LevelGraphNode*>(node);
    if(!levelNode)
    {
        bae::Logger::LogWarning("Node is not valid, GridPosition:" + std::to_string(gridPosition.Column) + " "
            + std::to_string(gridPosition.Row));
        return LevelNodeType::Block;
    }

    return levelNode->m_GridType;
}

void Game::LevelGridGraph::SetNodeType(const bae::Graphs::GridPosition& gridPosition, const LevelNodeType type) const
{
    const auto pNode = dynamic_cast<LevelGraphNode*>(GetNode(gridPosition));
    if(!pNode)
    {
        bae::Logger::LogWarning("Node is invalid, GridPosition:" + std::to_string(gridPosition.Column) + " "
            + std::to_string(gridPosition.Row));
        return;
    }

    pNode->m_GridType = type;
}

void Game::LevelGridGraph::SetNodeType(const glm::vec2& position, const LevelNodeType type) const
{
    const bae::Graphs::GridPosition gridPosition = GetGridPosition(position);
    return SetNodeType(gridPosition, type);
}

