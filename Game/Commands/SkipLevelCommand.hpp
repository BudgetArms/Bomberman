#pragma once


#include "Commands/Command.hpp"


namespace Game
{
    class SkipLevelCommand final : public bae::Command
    {
    public:
        SkipLevelCommand();

        void Execute() override;
    };
}

