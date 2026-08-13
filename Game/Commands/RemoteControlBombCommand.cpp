#include "RemoteControlBombCommand.hpp"

#include "Components/BombermanComponent.hpp"


using namespace Game;


RemoteControlBombCommand::RemoteControlBombCommand(bae::GameObject& owner) :
    GameActorCommand(owner)
{
}

void RemoteControlBombCommand::Execute()
{
    GetActor()->GetComponent<BombermanComponent>()->TryActivateBomb();
}

