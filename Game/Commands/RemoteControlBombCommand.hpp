#pragma once

#include "Commands/GameActorCommand.hpp"


namespace Game
{
    class RemoteControlBombCommand : public bae::GameActorCommand
    {
    public:
        explicit RemoteControlBombCommand(bae::GameObject& owner);

        void Execute() override;
    };
}

