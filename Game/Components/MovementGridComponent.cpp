#include "MovementGridComponent.hpp"

#include <glm/glm.hpp>

#include "Base/Events.hpp"
#include "Components/GridComponent.hpp"
#include "Singletons/GameTime.hpp"

#include "Managers/LevelManager.hpp"


using namespace Game;


MovementGridComponent::MovementGridComponent(bae::GameObject& owner) :
    Component(owner),
    Subject(owner)
{
    const auto& levelManager = LevelManager::GetInstance();
    m_GridComponent          = levelManager.GetGridComponent();
}

void MovementGridComponent::Update()
{
    if(!m_bWasDirectionSetThisFrame)
    {
        LockOnGrid();
        return;
    }
    m_bWasDirectionSetThisFrame = false;

    // Not Near Node & Opposite/Same Direction Then Last Correct Direction
    if(IsOppositeDirection(m_DirectionThisFrame, m_LastCorrectDirection) ||
        m_DirectionThisFrame == m_LastCorrectDirection)
    {
        if(!IsAtNodeCenter())
        {
            ApplyMovement();
            return;
        }

        if(!CanMoveInDirection(m_DirectionThisFrame))
        {
            return;
        }
    }

    if(!IsAtNodeCenter() || !CanMoveInDirection(m_DirectionThisFrame))
    {
        return;
    }

    ApplyMovement();
}

Direction MovementGridComponent::GetDirection() const
{
    return m_DirectionThisFrame;
}

void MovementGridComponent::SetDirection(const Direction direction)
{
    m_DirectionThisFrame        = direction;
    m_bWasDirectionSetThisFrame = true;
}

bool MovementGridComponent::CanMoveInDirection(const Direction direction) const
{
    const glm::vec2& position = m_Owner->GetWorldLocation();

    const bae::Graphs::GridPosition gridPosition = m_GridComponent->GetClosestValidNodePosition(position);

    if(gridPosition.Column < 0 || gridPosition.Row < 0 ||
        gridPosition.Row > m_GridComponent->GetRows() || gridPosition.Column > m_GridComponent->GetColumns())
    {
        return false;
    }

    if(!m_GridComponent->DoesConnectionExistInDirection(gridPosition, direction))
    {
        return false;
    }


    bae::Graphs::GridPosition moveToGridPosition = gridPosition;
    switch(direction)
    {
        case Direction::Right:
            ++moveToGridPosition.Column;
            break;
        case Direction::Left:
            --moveToGridPosition.Column;
            break;
        case Direction::Up:
            --moveToGridPosition.Row;
            break;
        case Direction::Down:
            ++moveToGridPosition.Row;
            break;
    }

    const LevelNodeType nodeType = m_GridComponent->GetNodeType(moveToGridPosition);
    return nodeType == LevelNodeType::Nothing;
}


void MovementGridComponent::ApplyMovement()
{
    const float directionMoved = m_Speed * bae::GameTime::GetInstance().GetDeltaTime();
    switch(m_DirectionThisFrame)
    {
        case Direction::Left:
            m_Owner->AddLocation({ -directionMoved, 0 });
            break;

        case Direction::Right:
            m_Owner->AddLocation({ directionMoved, 0 });
            break;

        case Direction::Up:
            m_Owner->AddLocation({ 0, -directionMoved });
            break;

        case Direction::Down:
            m_Owner->AddLocation({ 0, directionMoved });
            break;
    }

    if(m_LastCorrectDirection != m_DirectionThisFrame)
    {
        NotifyObservers(GetEventHash(Events::DirectionChanged));
    }

    m_LastCorrectDirection = m_DirectionThisFrame;

    LockOnGrid();
}

void MovementGridComponent::LockOnGrid() const
{
    glm::vec2 position = m_Owner->GetWorldLocation();

    const bae::Graphs::GridPosition gridPosition = m_GridComponent->GetClosestValidNodePosition(position);
    const glm::vec2 nodePosition                 = m_GridComponent->GetPosition(gridPosition);

    switch(m_LastCorrectDirection)
    {
        case Direction::Right:
        case Direction::Left:
            position.y = nodePosition.y;
            m_Owner->SetWorldLocation(position);
            break;
        case Direction::Up:
        case Direction::Down:
            position.x = nodePosition.x;
            m_Owner->SetWorldLocation(position);
            break;
    }
}

bool MovementGridComponent::IsAtNodeCenter() const
{
    const glm::vec2& position = m_Owner->GetWorldLocation();

    const auto gridPosition = m_GridComponent->GetClosestValidNodePosition(position);

    if(gridPosition.Column < 0 || gridPosition.Row < 0 ||
        gridPosition.Row > m_GridComponent->GetRows() || gridPosition.Column > m_GridComponent->GetColumns())
    {
        return false;
    }

    const glm::vec2 nodePosition = m_GridComponent->GetPosition(gridPosition);
    return glm::distance(position, nodePosition) <= m_ToleranceNodeCenter;
}

bool MovementGridComponent::IsOppositeDirection(const Direction direction, const Direction oppositeDirection)
{
    switch(direction)
    {
        case Direction::Right:
            if(oppositeDirection == Direction::Left)
            {
                return true;
            }
            break;
        case Direction::Left:
            if(oppositeDirection == Direction::Right)
            {
                return true;
            }
            break;
        case Direction::Up:
            if(oppositeDirection == Direction::Down)
            {
                return true;
            }
            break;
        case Direction::Down:
            if(oppositeDirection == Direction::Up)
            {
                return true;
            }
            break;
    }

    return false;
}

