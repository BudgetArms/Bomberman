#pragma once

#include "Base/DirectionEnum.hpp"
#include "Base/LevelNodeType.hpp"
#include "Components/Component.hpp"
#include "Graphs/AStar.hpp"
#include "Graphs/GridGraph.hpp"


namespace Game
{
    class LevelGridGraph;

    class GridComponent final : public bae::Component
    {
    public:
        GridComponent(bae::GameObject& owner, int columns, int rows, const glm::ivec2& cellSize);
        GridComponent(bae::GameObject& owner, const glm::vec2& gridSize, int columns, int rows);
        ~GridComponent() override;


        void Render() const override;

        // Nodes
        void AddNode(int nodeId) const;
        void AddNode(const bae::Graphs::GridPosition& position) const;
        void AddNodeAtPosition(const glm::vec2& positionNode) const;

        void RemoveNode(int nodeId) const;
        void RemoveNode(const bae::Graphs::GridPosition& position) const;
        void RemoveNodeAtPosition(const glm::vec2& positionNode) const;

        [[nodiscard]] LevelNodeType GetNodeType(int nodeId) const;
        void SetNodeType(int nodeId, LevelNodeType nodeType) const;

        // Connections
        void AddConnection(int nodeId, Direction direction) const;
        void AddConnection(const glm::vec2& positionNode, Direction direction) const;

        void RemoveConnection(int nodeId, Direction direction) const;
        void RemoveConnection(const glm::vec2& positionNode, Direction direction) const;

        void AddConnectionsToNeighbors(bae::Graphs::GridPosition position) const;


        [[nodiscard]] bool GetRenderCells() const;
        void SetRenderCells(bool bRenderCells) const;

        [[nodiscard]] bool GetRenderNodes() const;
        void SetRenderNodes(bool bRenderNodes) const;

        [[nodiscard]] bool GetRenderConnections() const;
        void SetRenderConnections(bool bRenderConnections) const;


        [[nodiscard]] bae::Graphs::GridPosition GetGridPosition(const glm::vec2& position) const;

        [[nodiscard]] glm::vec2 GetPosition(bae::Graphs::GridPosition gridPosition) const;

        [[nodiscard]] bool IsValidGridPosition(bae::Graphs::GridPosition gridPosition) const;

        [[nodiscard]] bae::Graphs::GridPosition GetClosestValidNodePosition(const glm::vec2& position) const;


        [[nodiscard]] int GetColumns() const;
        [[nodiscard]] int GetRows() const;


        [[nodiscard]] bool IsInGrid(const glm::vec2& position) const;
        [[nodiscard]] bool IsInGrid(const bae::Graphs::GridPosition& gridPosition) const;

        [[nodiscard]] bool DoesConnectionExistInDirection(bae::Graphs::GridPosition gridPosition,
                                                          Direction direction) const;

        [[nodiscard]] std::vector<glm::vec2> GetALlConnections() const;


        [[nodiscard]] std::vector<glm::vec2> GetShortestPath(int startNodeId, int endNodeId) const;
        [[nodiscard]] std::vector<glm::vec2> GetShortestPath(const glm::vec2& startPos,
                                                             const glm::vec2& endPos) const;

    private:
        std::unique_ptr<LevelGridGraph> m_LevelGridGraph;
        bae::Graphs::AStar m_AStar;
    };
}

