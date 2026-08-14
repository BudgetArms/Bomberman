#pragma once

#include <vector>

#include "Core/GameObject.hpp"

#include "Base/DirectionEnum.hpp"
#include "Components/MovementGridComponent.hpp"


namespace Game
{
    class EnemyMovement
    {
    public:
        EnemyMovement(bae::GameObject& owner, float speed, int intelligence, float changeDirectionUpChance);


        void Update();

    private:
        void ChooseDirection();

        void ChooseRandomDirection();
        void ChooseIntelligentDirection();


        bool ShouldChangeDirection() const;

        std::vector<Direction> GetValidDirections() const;
        std::vector<Direction> GetValidNonReverseDirections() const;
        static Direction GetOppositeDirection(Direction direction);

        Direction GetDirectionTowardsPlayer() const;

        static Direction GetDirectionFromDistance(const glm::vec2& difference);
        bae::GameObject* GetClosestPlayer() const;


        bae::GameObject* m_Owner;
        MovementGridComponent* m_MovementComponent{};

        const int m_IntelligenceThreshold{ 2 };
        int m_Intelligence{};

        Direction m_CurrentDirection{ Direction::Right };
        const float m_ChangeDirectionUpChance;
    };
}

