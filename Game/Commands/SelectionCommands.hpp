#pragma once

#include "Commands/GameActorCommand.hpp"
#include "Core/Subject.hpp"


namespace Game
{
    class SelectionUpCommand final : public bae::GameActorCommand, public bae::Subject
    {
    public:
        explicit SelectionUpCommand(bae::GameObject& gameObject);

        void Execute() override;
    };

    class SelectionDownCommand final : public bae::GameActorCommand, public bae::Subject
    {
    public:
        explicit SelectionDownCommand(bae::GameObject& gameObject);

        void Execute() override;
    };

    class SelectionConfirmedCommand final : public bae::GameActorCommand, public bae::Subject
    {
    public:
        explicit SelectionConfirmedCommand(bae::GameObject& gameObject);

        void Execute() override;
    };
}

