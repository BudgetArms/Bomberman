#include "GridComponent.hpp"

#include "Base/LevelGridGraph.hpp"
#include "Core/HelperFunctions.hpp"


using namespace Game;


GridComponent::GridComponent(bae::GameObject& owner, int columns, int rows, const glm::ivec2& cellSize) :
    Component(owner),
    m_LevelGridGraph{ std::make_unique<LevelGridGraph>(owner.GetWorldLocation(), columns, rows, cellSize) },
    m_AStar{ m_LevelGridGraph.get(), bae::Graphs::HeuristicFunctions::Chebyshev }
{
}

GridComponent::GridComponent(bae::GameObject& owner, const glm::vec2& gridSize, const int columns,
                             const int rows) :
    GridComponent(owner, columns, rows,
                  glm::ivec2(static_cast<float>(gridSize.x) / static_cast<float>(columns),
                             static_cast<float>(gridSize.y) / static_cast<float>(rows)))
{
}

GridComponent::~GridComponent() = default;


void GridComponent::Render() const
{
    m_LevelGridGraph->Render();
}


void GridComponent::AddNode(const int nodeId) const
{
    // Gets existing (grid locked) node position
    const glm::vec2 positionNode                 = m_LevelGridGraph->GetNodePos(nodeId);
    const bae::Graphs::GridPosition gridPosition = m_LevelGridGraph->GetGridPosition(nodeId);

    if(m_LevelGridGraph->IsWithinBounds(gridPosition))
    {
        m_LevelGridGraph->AddNode(std::make_unique<bae::Graphs::Node>(positionNode));
    }
}

void GridComponent::AddNode(const bae::Graphs::GridPosition& position) const
{
    if(!m_LevelGridGraph->IsWithinBounds(position))
    {
        std::cout << FUNCTION_NAME << " Failed! GridPosition not in Grid!" << '\n';
        return;
    }

    const int nodeId = m_LevelGridGraph->GetNodeId(position);
    AddNode(nodeId);
}

void GridComponent::AddNodeAtPosition(const glm::vec2& positionNode) const
{
    const int nodeId = m_LevelGridGraph->GetNodeIdAtPosition(positionNode);
    AddNode(nodeId);
}

void GridComponent::RemoveNode(const int nodeId) const
{
    m_LevelGridGraph->RemoveNode(nodeId);
}

void GridComponent::RemoveNode(const bae::Graphs::GridPosition& position) const
{
    if(!m_LevelGridGraph->IsWithinBounds(position))
    {
        std::cout << FUNCTION_NAME << " Failed! GridPosition not in Grid!" << '\n';
        return;
    }

    const int nodeId = m_LevelGridGraph->GetNodeId(position);
    RemoveNode(nodeId);
}

void GridComponent::RemoveNodeAtPosition(const glm::vec2& positionNode) const
{
    const int nodeId = m_LevelGridGraph->GetNodeIdAtPosition(positionNode);
    RemoveNode(nodeId);
}

void GridComponent::SetNodeType(int nodeId, LevelNodeType gridType)
{
    m_LevelGridGraph->SetNodeType(nodeId, gridType);
}


void GridComponent::AddConnection(const int nodeId, const Direction direction) const
{
    const bae::Graphs::GridPosition gridPosition = m_LevelGridGraph->GetGridPosition(nodeId);

    std::vector<glm::vec2> directions{};
    switch(direction)
    {
        case Direction::Left:
            directions.emplace_back(-1, 0);
            break;
        case Direction::Right:
            directions.emplace_back(1, 0);
            break;
        case Direction::Up:
            directions.emplace_back(0, -1);
            break;
        case Direction::Down:
            directions.emplace_back(0, 1);
            break;
    }

    m_LevelGridGraph->AddConnectionsInDirections(nodeId, gridPosition, directions);
}

void GridComponent::AddConnection(const glm::vec2& positionNode, const Direction direction) const
{
    const int nodeId = m_LevelGridGraph->GetNodeIdAtPosition(positionNode);
    AddConnection(nodeId, direction);
}

void GridComponent::RemoveConnection(const int nodeId, const Direction direction) const
{
    const bae::Graphs::GridPosition gridPosition = m_LevelGridGraph->GetGridPosition(nodeId);

    std::vector<glm::vec2> directions{};
    switch(direction)
    {
        case Direction::Left:
            directions.emplace_back(-1, 0);
            break;
        case Direction::Right:
            directions.emplace_back(1, 0);
            break;
        case Direction::Up:
            directions.emplace_back(0, -1);
            break;
        case Direction::Down:
            directions.emplace_back(0, 1);
            break;
    }

    m_LevelGridGraph->RemoveConnectionsInDirections(nodeId, gridPosition, directions);
}

void GridComponent::RemoveConnection(const glm::vec2& positionNode, const Direction direction) const
{
    const int nodeId = m_LevelGridGraph->GetNodeIdAtPosition(positionNode);
    RemoveConnection(nodeId, direction);
}

void GridComponent::AddConnectionsToNeighbors(const bae::Graphs::GridPosition position) const
{
    m_LevelGridGraph->AddConnectionsToAdjacentCells(position);
}

bool GridComponent::GetRenderCells() const
{
    return m_LevelGridGraph->m_bRenderCells;
}

void GridComponent::SetRenderCells(const bool bRenderCells) const
{
    m_LevelGridGraph->m_bRenderCells = bRenderCells;
}

bool GridComponent::GetRenderNodes() const
{
    return m_LevelGridGraph->m_bRenderNodes;
}

void GridComponent::SetRenderNodes(const bool bRenderNodes) const
{
    m_LevelGridGraph->m_bRenderNodes = bRenderNodes;
}

bool GridComponent::GetRenderConnections() const
{
    return m_LevelGridGraph->m_bRenderConnections;
}

void GridComponent::SetRenderConnections(const bool bRenderConnections) const
{
    m_LevelGridGraph->m_bRenderConnections = bRenderConnections;
}

bae::Graphs::GridPosition GridComponent::GetGridPosition(const glm::vec2& position) const
{
    return m_LevelGridGraph->GetGridPosition(position);
}

glm::vec2 GridComponent::GetPosition(const bae::Graphs::GridPosition gridPosition) const
{
    return m_LevelGridGraph->GetPosition(gridPosition);
}

bool GridComponent::IsValidGridPosition(const bae::Graphs::GridPosition gridPosition) const
{
    if(!m_LevelGridGraph->IsWithinBounds(gridPosition))
    {
        return false;
    }
    const bae::Graphs::Node* node = m_LevelGridGraph->GetNode(gridPosition);
    if(!node || !node->IsValid())
    {
        return false;
    }

    return m_LevelGridGraph->GetNodeId(gridPosition) != bae::Graphs::InvalidNodeID;
}

bae::Graphs::GridPosition GridComponent::GetClosestValidNodePosition(const glm::vec2& position) const
{
    return m_LevelGridGraph->GetClosestValidNodePositionAtPosition(position);
}

int GridComponent::GetColumns() const
{
    return m_LevelGridGraph->GetColumns();
}

int GridComponent::GetRows() const
{
    return m_LevelGridGraph->GetRows();
}


bool GridComponent::IsInGrid(const glm::vec2& position) const
{
    const bae::Graphs::GridPosition gridPosition = m_LevelGridGraph->GetGridPosition(position);
    return IsInGrid(gridPosition);
}

bool GridComponent::IsInGrid(const bae::Graphs::GridPosition& gridPosition) const
{
    return m_LevelGridGraph->IsWithinBounds(gridPosition);
}

bool GridComponent::DoesConnectionExistInDirection(const bae::Graphs::GridPosition gridPosition,
                                                   const Direction direction) const
{
    if(!IsInGrid(gridPosition))
    {
        return false;
    }

    bae::Graphs::GridPosition newGridPosition = gridPosition;
    switch(direction)
    {
        case Direction::Left:
            --newGridPosition.Column;
            break;
        case Direction::Right:
            ++newGridPosition.Column;
            break;
        case Direction::Up:
            --newGridPosition.Row;
            break;
        case Direction::Down:
            ++newGridPosition.Row;
            break;
    }

    if(!IsInGrid(newGridPosition))
    {
        return false;
    }

    const int nodeId    = m_LevelGridGraph->GetNodeId(gridPosition);
    const int newNodeId = m_LevelGridGraph->GetNodeId(newGridPosition);

    const bae::Graphs::Connection* const connection = m_LevelGridGraph->FindConnection(nodeId, newNodeId);
    if(!connection || !connection->IsValid())
    {
        return false;
    }

    return true;
}

std::vector<glm::vec2> GridComponent::GetALlConnections() const
{
    auto nodes = m_LevelGridGraph->GetActiveNodes();

    return bae::Graphs::ConvertNodesToPositions(nodes);
}


std::vector<glm::vec2> GridComponent::GetShortestPath(const int startNodeId, const int endNodeId) const
{
    const bae::Graphs::GridPosition startPos =
            m_LevelGridGraph->GetClosestValidNodePosition(m_LevelGridGraph->GetGridPosition(startNodeId));
    const bae::Graphs::GridPosition endPos =
            m_LevelGridGraph->GetClosestValidNodePosition(m_LevelGridGraph->GetGridPosition(endNodeId));

    const std::vector<bae::Graphs::Node*> pathNodes = m_AStar.FindPath(m_LevelGridGraph->GetNode(startPos),
                                                                       m_LevelGridGraph->GetNode(endPos));

    return bae::Graphs::ConvertNodesToPositions(pathNodes);
}


std::vector<glm::vec2> GridComponent::GetShortestPath(const glm::vec2& startPos, const glm::vec2& endPos) const
{
    const int startNodeId = m_LevelGridGraph->GetNodeIdAtPosition(startPos);
    const int endNodeId   = m_LevelGridGraph->GetNodeIdAtPosition(endPos);

    return GetShortestPath(startNodeId, endNodeId);
}

