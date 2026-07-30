#include "ForceDamageCommand.hpp"

#include "Components/LifeComponent.hpp"
#include "Core/HelperFunctions.hpp"


using namespace Game;


ForceDamageCommand::ForceDamageCommand(bae::GameObject& actor) :
    GameActorCommand(actor)
{
}

void ForceDamageCommand::Execute()
{
    if(!GetActor())
    {
        return;
    }

    const auto lifeComponent = GetActor()->GetComponent<LifeComponent>();
    lifeComponent->RemoveLife();
    std::cout << FUNCTION_NAME << ": Removed one life, lives left: " << lifeComponent->GetLives() << '\n';
}

