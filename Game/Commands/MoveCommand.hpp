#pragma once

#include "Commands/GameActorCommand.hpp"

#include "Base/DirectionEnum.hpp"


namespace Game
{
    class MoveCommand : public bae::GameActorCommand
    {
    public:
        MoveCommand(bae::GameObject& owner, Direction direction);

        void Execute() override;

    private:
        Direction m_Direction;
    };
}

