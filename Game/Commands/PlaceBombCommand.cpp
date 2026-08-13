#include "PlaceBombCommand.hpp"

#include "Components/BombermanComponent.hpp"


using namespace Game;


PlaceBombCommand::PlaceBombCommand(bae::GameObject& owner) :
    GameActorCommand(owner)
{
}

void PlaceBombCommand::Execute()
{
    GetActor()->GetComponent<BombermanComponent>()->TryPlaceBomb();
}

