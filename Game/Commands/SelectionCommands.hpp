#pragma once

#include "Commands/GameActorCommand.hpp"
#include "Core/Subject.hpp"


namespace Game
{
    class SelectionDownCommand final : public bae::GameActorCommand, public bae::Subject
    {
    public:
        explicit SelectionDownCommand(bae::GameObject& gameObject);

        void Execute() override;
    };

    class SelectionUpCommand final : public bae::GameActorCommand, public bae::Subject
    {
    public:
        explicit SelectionUpCommand(bae::GameObject& gameObject);

        void Execute() override;
    };

    class SelectionLeftCommand final : public bae::GameActorCommand, public bae::Subject
    {
    public:
        explicit SelectionLeftCommand(bae::GameObject& gameObject);

        void Execute() override;
    };

    class SelectionRightCommand final : public bae::GameActorCommand, public bae::Subject
    {
    public:
        explicit SelectionRightCommand(bae::GameObject& gameObject);

        void Execute() override;
    };

    class SelectionConfirmedCommand final : public bae::GameActorCommand, public bae::Subject
    {
    public:
        explicit SelectionConfirmedCommand(bae::GameObject& gameObject);

        void Execute() override;
    };
}

