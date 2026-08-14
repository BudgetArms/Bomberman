#pragma once

#include "Components/Component.hpp"
#include "Core/Subject.hpp"

#include "Base/DirectionEnum.hpp"
#include "Components/GridComponent.hpp"


namespace Game
{
    class MovementGridComponent : public bae::Component, public bae::Subject
    {
    public:
        explicit MovementGridComponent(bae::GameObject& owner);

        void Update() override;

        void Enable();
        void Disable();

        [[nodiscard]] Direction GetDirection() const;
        void SetDirection(Direction direction);

        [[nodiscard]] bool CanMoveInDirection(Direction direction) const;

        float m_Speed{ 100.f };
        bool m_bIsDebuggingEnabled{};

    private:
        void ApplyMovement();

        void LockOnGrid() const;


        [[nodiscard]] bool IsAtNodeCenter() const;

        [[nodiscard]] static bool IsOppositeDirection(Direction direction, Direction oppositeDirection);

        void PrintDebugMessage(const std::string& message) const;

        bool m_bIsEnabled{ true };

        const float m_ToleranceNodeCenter{ 20.f };
        GridComponent* m_GridComponent{};
        Direction m_DirectionThisFrame{};
        Direction m_LastCorrectDirection{};
        bool m_bWasDirectionSetThisFrame{};
    };
}
