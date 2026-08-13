#pragma once

#include "Commands/GameActorCommand.hpp"


namespace Game
{
    class PlaceBombCommand : public bae::GameActorCommand
    {
    public:
        explicit PlaceBombCommand(bae::GameObject& owner);

        void Execute() override;
    };
}

