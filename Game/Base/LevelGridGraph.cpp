#include "LevelGridGraph.hpp"

#include <iostream>

#include "LevelGraphNode.hpp"
#include "Core/HelperFunctions.hpp"


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
        std::cout << FUNCTION_NAME << " Failed! Node is not valid, GridPosition:" << gridPosition.Column << " " <<
                gridPosition.Row << '\n';
        return LevelNodeType::Block;
    }

    return levelNode->m_GridType;
}

void Game::LevelGridGraph::SetNodeType(const bae::Graphs::GridPosition& gridPosition, const LevelNodeType type) const
{
    const auto pNode = dynamic_cast<LevelGraphNode*>(GetNode(gridPosition));
    if(!pNode)
    {
        std::cout << FUNCTION_NAME << " Failed! Node is not valid, GridPosition:" << gridPosition.Column << " " <<
                gridPosition.Row << '\n';
        return;
    }

    pNode->m_GridType = type;
}

void Game::LevelGridGraph::SetNodeType(const glm::vec2& position, const LevelNodeType type) const
{
    const bae::Graphs::GridPosition gridPosition = GetGridPosition(position);
    return SetNodeType(gridPosition, type);
}

