#pragma once


#include "Commands/Command.hpp"


namespace Game
{
    class ToggleMuteSoundsCommand final : public bae::Command
    {
    public:
        ToggleMuteSoundsCommand();

        void Execute() override;

    private:
        bool m_bIsMuted{ false };
    };
}

