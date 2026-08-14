#include "EnemyMovement.hpp"

#include <array>
#include <limits>

#include "Core/HelperFunctions.hpp"

#include "Components/MovementGridComponent.hpp"
#include "Managers/LevelManager.hpp"


using namespace Game;


EnemyMovement::EnemyMovement(bae::GameObject& owner, const float speed, const int intelligence,
                             const float changeDirectionUpChance) :
    m_Owner{ &owner },
    m_Intelligence{ intelligence },
    m_ChangeDirectionUpChance{ changeDirectionUpChance }
{
    m_MovementComponent = m_Owner->GetComponent<MovementGridComponent>();

    if(!m_MovementComponent)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string("EnemyMovement requires MovementGridComponent"));
    }

    m_MovementComponent->m_Speed = speed;
}


void EnemyMovement::Update()
{
    if(!m_MovementComponent)
    {
        return;
    }

    if(m_MovementComponent->IsAtNodeCenter())
    {
        if(!m_MovementComponent->CanMoveInDirection(m_CurrentDirection))
        {
            ChooseDirection();
        }
        else if(ShouldChangeDirection())
        {
            ChooseDirection();
        }
    }

    m_MovementComponent->SetDirection(m_CurrentDirection);
}


void EnemyMovement::ChooseDirection()
{
    if(m_Intelligence < m_IntelligenceTresshold)
    {
        ChooseRandomDirection();
        return;
    }

    ChooseIntelligentDirection();
}


void EnemyMovement::ChooseRandomDirection()
{
    std::vector<Direction> validDirections = GetValidNonReverseDirections();
    if(validDirections.empty())
    {
        validDirections = GetValidDirections();
    }

    if(validDirections.empty())
    {
        return;
    }

    const int randomIndex = bae::GetRandomInt(0, static_cast<int>(validDirections.size() - 1));
    m_CurrentDirection    = validDirections[randomIndex];
}


void EnemyMovement::ChooseIntelligentDirection()
{
    const bae::GameObject* player      = GetClosestPlayer();
    const glm::vec2 directionToPlayer  = player->GetWorldLocation() - m_Owner->GetWorldLocation();
    const Direction preferredDirection = GetDirectionFromDistance(directionToPlayer);

    if(m_MovementComponent->CanMoveInDirection(preferredDirection))
    {
        m_CurrentDirection = preferredDirection;
        return;
    }

    const std::vector<Direction> validDirections = GetValidNonReverseDirections();
    if(validDirections.empty())
    {
        ChooseRandomDirection();
        return;
    }

    const glm::vec2 enemyPosition  = m_Owner->GetWorldLocation();
    const glm::vec2 playerPosition = player->GetWorldLocation();

    Direction bestDirection = validDirections.front();

    float bestDistance = std::numeric_limits<float>::max();
    for(const Direction direction : validDirections)
    {
        glm::vec2 directionVector{};

        switch(direction)
        {
            case Direction::Left:
                directionVector.x = -1.f;
                break;

            case Direction::Right:
                directionVector.x = 1.f;
                break;

            case Direction::Up:
                directionVector.y = -1.f;
                break;

            case Direction::Down:
                directionVector.y = 1.f;
                break;
        }

        const glm::vec2 projectedPosition = enemyPosition + directionVector;
        const float distance              = glm::distance(projectedPosition, playerPosition);

        if(distance < bestDistance)
        {
            bestDistance  = distance;
            bestDirection = direction;
        }
    }

    m_CurrentDirection = bestDirection;
}

bool EnemyMovement::ShouldChangeDirection() const
{
    const float randomChance = bae::GetRandomFloat(0.f, 1.f);
    return randomChance < m_ChangeDirectionUpChance;
}


std::vector<Direction> EnemyMovement::GetValidDirections() const
{
    constexpr std::array directions
    {
        Direction::Up,
        Direction::Down,
        Direction::Left,
        Direction::Right
    };

    std::vector<Direction> validDirections{};

    for(const Direction direction : directions)
    {
        if(m_MovementComponent->CanMoveInDirection(direction))
        {
            validDirections.push_back(direction);
        }
    }

    return validDirections;
}


std::vector<Direction> EnemyMovement::GetValidNonReverseDirections() const
{
    std::vector<Direction> result{};

    const std::vector<Direction> validDirections = GetValidDirections();
    const Direction oppositeDirection            = GetOppositeDirection(m_CurrentDirection);

    for(const Direction direction : validDirections)
    {
        if(direction != oppositeDirection)
        {
            result.push_back(direction);
        }
    }

    return result;
}


Direction EnemyMovement::GetOppositeDirection(const Direction direction)
{
    switch(direction)
    {
        case Direction::Left:
            return Direction::Right;

        case Direction::Right:
            return Direction::Left;

        case Direction::Up:
            return Direction::Down;

        case Direction::Down:
            return Direction::Up;
    }

    std::cout << FUNCTION_NAME << " This should never be reached" << '\n';
    throw std::runtime_error(FUNCTION_NAME + std::string("Failed! This should never be reached"));
}


Direction EnemyMovement::GetDirectionTowardsPlayer() const
{
    const glm::vec2 difference = GetClosestPlayer()->GetWorldLocation() - m_Owner->GetWorldLocation();
    return GetDirectionFromDistance(difference);
}


Direction EnemyMovement::GetDirectionFromDistance(const glm::vec2& distanceToPlayer)
{
    if(std::abs(distanceToPlayer.x) > std::abs(distanceToPlayer.y))
    {
        if(distanceToPlayer.x > 0.f)
        {
            return Direction::Right;
        }

        return Direction::Left;
    }

    if(distanceToPlayer.y > 0.f)
    {
        return Direction::Down;
    }

    return Direction::Up;
}


bae::GameObject* EnemyMovement::GetClosestPlayer() const
{
    bae::GameObject* closestPlayer{};

    float closestDistance         = std::numeric_limits<float>::max();
    const glm::vec2 enemyPosition = m_Owner->GetWorldLocation();

    const auto players = LevelManager::GetInstance().GetPlayers();
    for(const auto& player : players | std::views::keys)
    {
        const float distance = glm::distance(enemyPosition, player->GetWorldLocation());

        if(distance < closestDistance)
        {
            closestDistance = distance;
            closestPlayer   = player;
        }
    }

    return closestPlayer;
}

