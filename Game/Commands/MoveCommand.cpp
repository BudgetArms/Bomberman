#include "MoveCommand.hpp"

#include "Components/MovementGridComponent.hpp"


using namespace Game;

MoveCommand::MoveCommand(bae::GameObject& owner, const Direction direction) :
    GameActorCommand(owner),
    m_Direction{ direction }
{
}

void MoveCommand::Execute()
{
    GetActor()->GetComponent<MovementGridComponent>()->SetDirection(m_Direction);
}

