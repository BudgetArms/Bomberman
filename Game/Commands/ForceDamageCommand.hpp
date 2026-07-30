#pragma once

#include "Commands/GameActorCommand.hpp"


namespace Game
{
    class ForceDamageCommand : public bae::GameActorCommand
    {
    public:
        explicit ForceDamageCommand(bae::GameObject& actor);

        void Execute() override;
    };
}

