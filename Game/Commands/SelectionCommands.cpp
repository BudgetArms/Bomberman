#include "SelectionCommands.hpp"

#include "Base/Events.hpp"


using namespace Game;


SelectionDownCommand::SelectionDownCommand(bae::GameObject& gameObject) :
    GameActorCommand(gameObject),
    Subject(gameObject)
{
}

void SelectionDownCommand::Execute()
{
    NotifyObservers(bae::EventData(GetEventHash(Events::SelectionDown)));
}


SelectionUpCommand::SelectionUpCommand(bae::GameObject& gameObject) :
    GameActorCommand(gameObject),
    Subject(gameObject)
{
}

void SelectionUpCommand::Execute()
{
    NotifyObservers(bae::EventData(GetEventHash(Events::SelectionUp)));
}


SelectionLeftCommand::SelectionLeftCommand(bae::GameObject& gameObject) :
    GameActorCommand(gameObject),
    Subject(gameObject)
{
}

void SelectionLeftCommand::Execute()
{
    NotifyObservers(bae::EventData(GetEventHash(Events::SelectionLeft)));
}


SelectionRightCommand::SelectionRightCommand(bae::GameObject& gameObject) :
    GameActorCommand(gameObject),
    Subject(gameObject)
{
}

void SelectionRightCommand::Execute()
{
    NotifyObservers(bae::EventData(GetEventHash(Events::SelectionRight)));
}


SelectionConfirmedCommand::SelectionConfirmedCommand(bae::GameObject& gameObject) :
    GameActorCommand(gameObject),
    Subject(gameObject)
{
}

void SelectionConfirmedCommand::Execute()
{
    NotifyObservers(bae::EventData(GetEventHash(Events::SelectionConfirmed)));
}

