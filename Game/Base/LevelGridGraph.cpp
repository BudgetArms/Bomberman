#include "LevelGridGraph.hpp"

#include <iostream>

#include "LevelGraphNode.hpp"
#include "Core/HelperFunctions.hpp"


Game::LevelGridGraph::LevelGridGraph(const glm::vec2& position, const int columns, const int rows,
                                     const glm::ivec2& cellSize) :
    GridGraph(position, columns, rows, cellSize, false, false, nullptr)
{
}

void Game::LevelGridGraph::SetNodeType(const int nodeId, const LevelNodeType type) const
{
    const auto pNode = dynamic_cast<LevelGraphNode*>(GetNode(GetGridPosition(nodeId)));
    if(!pNode)
    {
        std::cout << FUNCTION_NAME << " Failed! Node is not valid, NodeId:" << nodeId << '\n';
        return;
    }

    pNode->m_GridType = type;
}

void Game::LevelGridGraph::SetNodeType(const glm::vec2& position, const LevelNodeType type) const
{
    const int nodeId = GetNodeId(GetGridPosition(position));
    return SetNodeType(nodeId, type);
}

