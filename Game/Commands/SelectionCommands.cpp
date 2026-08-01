#include "SelectionCommands.hpp"

#include "Base/Events.hpp"


using namespace Game;


SelectionUpCommand::SelectionUpCommand(bae::GameObject& gameObject) :
    GameActorCommand(gameObject),
    Subject(gameObject)
{
}

void SelectionUpCommand::Execute()
{
    NotifyObservers(GetEventHash(Events::SelectionUp));
}


SelectionDownCommand::SelectionDownCommand(bae::GameObject& gameObject) :
    GameActorCommand(gameObject),
    Subject(gameObject)
{
}

void SelectionDownCommand::Execute()
{
    NotifyObservers(GetEventHash(Events::SelectionUp));
}


SelectionLeftCommand::SelectionLeftCommand(bae::GameObject& gameObject) :
    GameActorCommand(gameObject),
    Subject(gameObject)
{
}

void SelectionLeftCommand::Execute()
{
    NotifyObservers(GetEventHash(Events::SelectionLeft));
}


SelectionRightCommand::SelectionRightCommand(bae::GameObject& gameObject) :
    GameActorCommand(gameObject),
    Subject(gameObject)
{
}

void SelectionRightCommand::Execute()
{
    NotifyObservers(GetEventHash(Events::SelectionRight));
}


SelectionConfirmedCommand::SelectionConfirmedCommand(bae::GameObject& gameObject) :
    GameActorCommand(gameObject),
    Subject(gameObject)
{
}

void SelectionConfirmedCommand::Execute()
{
    NotifyObservers(GetEventHash(Events::SelectionUp));
}
