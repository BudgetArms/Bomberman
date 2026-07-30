#pragma once

#include "Commands/GameActorCommand.hpp"

#include "Base/DirectionEnum.hpp"


namespace Game
{
    class ForceMoveCommand final : public bae::GameActorCommand
    {
    public:
        ForceMoveCommand(bae::GameObject& owner, Direction direction, float speed);

        void Execute() override;

    private:
        Direction m_Direction;
        const float m_Speed;
    };
}
