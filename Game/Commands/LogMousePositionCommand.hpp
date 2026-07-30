#pragma once

#include "Commands/Command.hpp"


namespace Game
{
    class LogMousePositionCommand final : public bae::Command
    {
    public:
        LogMousePositionCommand() = default;

        void Execute() override;
    };
}
